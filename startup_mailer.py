import subprocess
import smtplib
from email.mime.text import MIMEText
import datetime

# Sender Account Information
gmail_user = 'xbotmessenger@gmail.com' 			# Email to send from
gmail_password = 'timrobot' 				# Gmail password
smtpserver = smtplib.SMTP('smtp.gmail.com', 587) 	# Gmail server info

# Emails to send info to
to = ['ajaysriv3@gmail.com']
#to.append('as1877@rutgers.edu')
to.append('7328533481@vtext.com')
#to.append('7327543448@txt.att.net')
#to.append('7326476631@txt.att.net')

# Connect to email server
smtpserver.ehlo()  					# Says 'hello' to server
smtpserver.starttls()  					# Start TLS encryption
smtpserver.ehlo()
smtpserver.login(gmail_user, gmail_password)		# Login
today = datetime.date.today()

# Get SSID
p=subprocess.Popen('iwconfig enx801f027c7bae', shell=True, stdout=subprocess.PIPE)
data = p.communicate()
iwconfig = data[0].splitlines()
iwconfig = iwconfig[0].split()
ssid = iwconfig[iwconfig.index('802.11bgn')+1]

# Get IP
p=subprocess.Popen('ip route list', shell=True, stdout=subprocess.PIPE)
data = p.communicate()
ipLines = data[0].splitlines()
ipLines = ipLines[2].split()
ip = ipLines[ipLines.index('src')+1]

# Create a sentence for each data item
ssidSentence = 'xBot is connected to:   %s' % (ssid)
ipSentence = 'xBot\'s IP is:   %s' % (ip)

# Create email to send
toaddr = str(to[0])
del to[0]
cc = to
fromaddr = gmail_user
message_subject = 'xBot\'s Network Info on %s' % today.strftime('%b %d %Y')
message_text = ssidSentence + "\n" + ipSentence + "\n"
message = "From: %s\r\n" % fromaddr + "To: %s\r\n" % toaddr + "CC: %s\r\n" % ",".join(cc) + "Subject: %s\r\n" % message_subject + "\r\n" + message_text
toaddrs = [toaddr] + cc

# Send email & disconnect from server
smtpserver.sendmail(fromaddr, toaddrs, message)
smtpserver.quit()
