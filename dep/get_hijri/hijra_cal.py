import datetime
import requests 
import json

#Fetch current day
now = datetime.datetime.now()
input = str(now.day) + '-' + str(now.month) + '-' + str(now.year)

# Make a get request to get the latest position of the ISS from the OpenNotify API.
response = requests.get("https://api.aladhan.com/v1/gToH?date=" + input)

# Parse the response
data = response.json()

# Print the contents of the response (the data the server returned)
print(data['data']['hijri']['date'])


