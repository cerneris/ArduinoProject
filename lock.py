import serial
import time
import smtplib
import random
import os
from picamera import PiCamera
from email.mime.text import MIMEText
from email.mime.image import MIMEImage
from email.mime.multipart import MIMEMultipart

class lock:
	# Send email with an image and a message attached.		
	def send_mail_image(ImgFileName, smtp_server, port, sender_email, password, receiver_email, message):
		print("Sending email")
		img_data = open(ImgFileName, 'rb').read()
		msg = MIMEMultipart()
		msg['Subject'] = 'LockPassword'
		msg['From'] = sender_email
		msg['To'] = receiver_email
		
		text = MIMEText(message)
		msg.attach(text)
		image = MIMEImage(img_data, name=os.path.basename(ImgFileName))
		msg.attach(image)
		
		s = smtplib.SMTP(smtp_server, port)
		s.ehlo()
		s.starttls()
		s.ehlo()
		s.login(sender_email, password)
		s.sendmail(sender_email, receiver_email, msg.as_string())
		s.quit()

	# Send email with only message attached.	
	def send_mail(smtp_server, port, sender_email, password, receiver_email, message):
		print("Sending email")
		msg = MIMEMultipart()
		msg['Subject'] = 'Arduino Password'
		msg['From'] = sender_email
		msg['To'] = receiver_email
		
		text = MIMEText(message)
		msg.attach(text)
		s = smtplib.SMTP(smtp_server, port)
		s.ehlo()
		s.starttls()
		s.ehlo()
		s.login(sender_email, password)
		s.sendmail(sender_email, receiver_email, msg.as_string())
		s.quit()

	def main(self):
		try:
			# Open serial connection.
			ser = serial.Serial('/dev/ttyACM0', baudrate=9600)
			camera = PiCamera()
			time.sleep(2)
			print("Ready to work...")
			
			# Set parameters for SMTP email.
			port = 587
			smtp_server = "smtp.gmail.com"
			sender_email = "arduinolock1234@gmail.com"
			receiver_email = "razorzxz@fastmail.fm"
			password = "ArduinoPassword123"
			message1 = "2-state authentication password: "
			message2 = "Unauthorized access attempt detected, generating new password: "
			ser.flushInput()
			command = '0'
			
			# Main while loop that waits for serial data.
			# Arduino sends 1 or 2 depending on the state it is in.
			# If Arduino is waiting for 2-state authentication.
			# If Arduino is in full-lock mode. (Unauthorized access attempt)
			while(1):
				while ser.inWaiting():
					print("Reading serial")
					command = ser.read()
					time.sleep(1)
				if command == '1':
					print("Running command 1")
					new_password = str(random.randint(1000, 9999))
					message1 = message1 + new_password
					ser.write(new_password)
					send_mail(smtp_server, port, sender_email, password, receiver_email, message1)
					# Send email to user of password
					# Send new password to Arduino
					command = '0'
					time.sleep(5)
					print("Done")
				elif command == '2':	
					print("Running command 2")
					new_password = str(random.randint(1000, 9999))
					message2 = message2 + new_password
					ser.write(new_password)
					camera.capture("/home/pi/Desktop/image.jpg")
					time.sleep(2)
					send_mail_image("/home/pi/Desktop/image.jpg", smtp_server, port, sender_email, password, receiver_email, message2)
					# Take photo of culprit
					# Send reset password to email.
					# Send reset password to Arduino.
					command = '0'
					time.sleep(5)
					print("Done")
		except KeyboardInterrupt:
			print("Exiting program...")
			
if __name__ == '__main__':
	lukko = lock()
	lukko.main()
