#!/usr/bin/python
import thingspeak #library for accessing ThingSpeak.com
import json #library used to interpret the json response from ThingSpeak
from datetime import datetime #library used to manipulte time stamps
import time
import smtplib #library used to send emails
import Private #stores all sensitive information
from requests.exceptions import ConnectionError

ch = thingspeak.Channel(234851, api_key='V9E6SE1V7ZY60S8C')

def sendEmail(subject, message, severity):
    SMTP_SERVER = 'smtp.gmail.com'
    SMTP_PORT = 587
    GMAIL_USERNAME = Private.gmailUsername
    GMAIL_PASSWORD = Private.gmailPassword

    if severity == 0:
        recipient = Private.warningRecipient
        recipients = Private.warningRecipients
    elif severity == 1:
        recipient = Private.alarmRecipient
        recipients = Private.alarmRecipients

    emailText = "" + message + ""
    headers = ["From: " + GMAIL_USERNAME,
               "Subject: " + subject,
               "To: " + recipient,
               "MIME-Version: 1.0",
               "Content-Type: text/html"]
    headers = "\r\n".join(headers)

    session = smtplib.SMTP(SMTP_SERVER, SMTP_PORT)

    session.ehlo()
    session.starttls()
    session.ehlo

    session.login(GMAIL_USERNAME, GMAIL_PASSWORD)

    session.sendmail(GMAIL_USERNAME, recipients, headers + "\r\n\r\n" + emailText)
    session.quit()

def getThingSpeakData():
    results = ch.get({'results': 1, 'status': 'true'})
    parsed_results = json.loads(results)
    return parsed_results

def getFireStatus(data):
    status = data['feeds'][0]['status']
    return status.find('Fire') > -1

def getAirStatus(data):
    status = data['feeds'][0]['status']
    return status.find('Air Temp High') > -1

def getOilStatus(data):
    status = data['feeds'][0]['status']
    return status.find('Oil Temp High') > -1

def getElapsedTime(data):
    date_string = data['feeds'][0]['created_at']
    date = datetime.strptime(date_string, '%Y-%m-%dT%H:%M:%SZ')
    currentDate = datetime.utcnow()
    elapsedTime = (currentDate - date).total_seconds()
    return elapsedTime

def getCurrentDateTimeString():
    currentTime = datetime.utcnow()
    formatedTime = currentTime.strftime("%d-%b-%Y %H:%M:%S UTC")
    return formatedTime

def getDataForEmail(data):
    # str = "<br><br>"
    # date_string = data['feeds'][0]['created_at']
    # date = datetime.strptime(date_string, '%Y-%m-%dT%H:%M:%SZ')
    # str = "<h3>%s UTC<br>" % date
    str = "<h3>Last Entry ID: %s<br>" % data['feeds'][0]['entry_id']
    str += "Status: %s</h3>" % data['feeds'][0]['status']
    str += "<b>Error States:</b>"
    str += "<ul>"
    str += "<li>On Fire: %s</li>" % getFireStatus(data)
    str += "<li>Air High: %s</li>" % getAirStatus(data)
    str += "<li>Oil High: %s</li>" % getOilStatus(data)
    str += "<ul>"
    str += "<b>Data:</b>"
    str += "<ul>"
    str += "<li>Air Temp: %s</li>" % data['feeds'][0]['field1']
    str += "<li>Oil Temp: %s</li>" % data['feeds'][0]['field2']
    str += "<li>Flame Sensor: %s</li>" % data['feeds'][0]['field3']
    str += "</ul>"
    str += "<br><br><b>Note:</b> All times are in UTC"
    return str

def assembleEmailMessage(msgString, data):
    message = "<h2>" + msgString + "</h2>"
    message += getDataForEmail(data)
    return message

oldFireStatus = False
oldAirStatus = False
oldOilStatus = False
fireEmailSent = False
airEmailSent = False
oilEmailSent = False
timeEmailSent = False
errorEmailSent = False
while True:
    # Gather the data to act on
    try:
        data = getThingSpeakData()
    except:# ConnectionError as err:
        # print 'Handling Connection Error:', err
        time.sleep(2)
    else:
        onFire = getFireStatus(data)
        airHigh = getAirStatus(data)
        oilHigh = getOilStatus(data)
        elapsedTime = getElapsedTime(data)
        if onFire:
            severity = 1
        else:
            severity = 0

        # Send an email if the time since the last data post is more than 5 minutes
        if elapsedTime > 300 and not(timeEmailSent):
            subject = "Error: Fume Hood Not Reporting"
            message = "There have been no posts to ThingSpeak from the fume hood in the last 5 minutes"
            message = assembleEmailMessage(message, data)
            sendEmail(subject, message, severity)
            timeEmailSent = True
            print "Time Email sent at %s" % datetime.utcnow()
        elif elapsedTime <= 300 and timeEmailSent:
            subject = "Clear: Fume Hood Started Reporting"
            message = "The fume hood monitor has begun again to post to ThingSpeak"
            message = assembleEmailMessage(message, data)
            sendEmail(subject, message, severity)
            timeEmailSent = False
            print "Time Email cleared sent at %s" % datetime.utcnow()

        # Send an email if there is a fire
        if (onFire) and not(fireEmailSent):
            subject = "Error: On Fire - %s" % getCurrentDateTimeString()
            message = "The Fume Hood is reporting a fire"
            message = assembleEmailMessage(message, data)
            sendEmail(subject, message, severity)
            fireEmailSent = True
            errorEmailSent = True
            print "Fire Email sent at %s" % datetime.utcnow()
        # Send an email when the fire state clears
        elif not(onFire) and fireEmailSent:
            # subject = "Clear: On Fire - %s" % getCurrentDateTimeString()
            # message = "Fire state has been cleared"
            # message = assembleEmailMessage(message, data)
            # sendEmail(subject, message, severity)
            fireEmailSent = False
            # print "Fire Cleared Email sent at %s" % datetime.utcnow()

        # Send an email if there is an air high alarm
        if (airHigh) and not(airEmailSent):
            subject = "Error: Air High Temp - %s" % getCurrentDateTimeString()
            message = "The Fume Hood is reporting that the air temperature is exceeding its max limit"
            message = assembleEmailMessage(message, data)
            sendEmail(subject, message, severity)
            airEmailSent = True
            errorEmailSent = True
            print "Air Email sent at %s" % datetime.utcnow()
        # Send an email when the air high alarm clears
        elif not(airHigh) and airEmailSent:
            # subject = "Clear: Air High Temp - %s" % getCurrentDateTimeString()
            # message = "The air temperature has cooled sufficiently"
            # message = assembleEmailMessage(message, data)
            # sendEmail(subject, message, severity)
            airEmailSent = False
            # print "Air Cleared Email sent at %s" % datetime.utcnow()

        # Send an email if there is an oil high alarm
        if (oilHigh) and not(oilEmailSent):
            subject = "Error: Oil High Temp - %s" % getCurrentDateTimeString()
            message = "The Fume Hood is reporting that the oil temperature is exceeding its max limit"
            message = assembleEmailMessage(message, data)
            sendEmail(subject, message, severity)
            oilEmailSent = True
            errorEmailSent = True
            print "Oil Email sent at %s" % datetime.utcnow()
        # Send an email when the oil high alarm clears
        elif not(oilHigh) and oilEmailSent:
            # subject = "Clear: Oil High Temp - %s" % getCurrentDateTimeString()
            # message = "The oil temperature has cooled sufficiently"
            # message = assembleEmailMessage(message, data)
            # sendEmail(subject, message, severity)
            oilEmailSent = False
            # print "Oil Cleared Email sent at %s" % datetime.utcnow()

        if not(onFire) and not(airHigh) and not(oilHigh) and errorEmailSent:
            subject = "Clear: All system are normal - %s" % getCurrentDateTimeString()
            message = "All system errors have been cleared and power has been restored to the hot plate"
            message = assembleEmailMessage(message, data)
            sendEmail(subject, message, severity)
            errorEmailSent = False
            print "All clear email sent as %s" % datetime.utcnow()

        time.sleep(1.5)
