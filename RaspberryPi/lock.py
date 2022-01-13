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
	def __init__(self):
		# Open serial connection.
		self.ser = serial.Serial('/dev/ttyACM0', baudrate=9600)
		self.camera = PiCamera()
		time.sleep(2)
		print("Ready to work...")
		
		# Set parameters for SMTP email.
		self.port = 587
		self.smtp_server = "smtp.gmail.com"
		self.sender_email = 
		self.receiver_email = 
		self.password = 
		self.message1 = "2-state authentication password: "
		self.message2 = "Unauthorized access attempt detected, generating new password: "
		self.ser.flushInput()
		self.command = '0'	
	def password_gen(self):
		password = ""
		for i in range(0, 4):
			password += str(random.randint(0, 9))
		return password
	# Send email with an image and a message attached.		
	def send_mail_image(self, ImgFileName, smtp_server, port, sender_email, password, receiver_email, message):
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
	def send_mail(self, smtp_server, port, sender_email, password, receiver_email, message):
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
			# Main while loop that waits for serial data.
			# Arduino sends 1 or 2 depending on the state it is in.
			# If Arduino is waiting for 2-state authentication.
			# If Arduino is in full-lock mode. (Unauthorized access attempt)
			while(1):
				while self.ser.inWaiting():
					print("Reading serial")
					self.command = self.ser.read()
					self.ser.flush()
					time.sleep(1)
				if self.command == '1':
					print("Running command 1")
					new_password = self.password_gen()
					message = self.message1 + new_password
					self.ser.write(new_password)
					time.sleep(1)
					self.ser.flush()
					self.send_mail(self.smtp_server, self.port, self.sender_email, self.password, self.receiver_email, message)
					# Send email to user of password
					# Send new password to Arduino
					self.command = '0'
					time.sleep(2)
					print("Done")
				elif self.command == '2':	
					print("Running command 2")
					new_password = self.password_gen()
					message = self.message2 + new_password
					self.ser.write(new_password)
					time.sleep(1)
					self.ser.flush()
					self.camera.capture("/home/pi/Desktop/image.jpg")
					time.sleep(2)
					self.send_mail_image("/home/pi/Desktop/image.jpg", self.smtp_server, self.port, self.sender_email, self.password, self.receiver_email, message)
					# Take photo of culprit
					# Send reset password to email.
					# Send reset password to Arduino.
					self.command = '0'
					time.sleep(2)
					print("Done")
		except KeyboardInterrupt:
			print("Exiting program...")
			
if __name__ == '__main__':
	lukko = lock()
	lukko.main()
