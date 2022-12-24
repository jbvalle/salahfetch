import requests 
import json
import os

APIKEY = os.environ.get('ACCU_WEATHER_APIKEY')

response = requests.get("http://dataservice.accuweather.com/forecasts/v1/hourly/1hour/31868?apikey=" + APIKEY);

# Parse the response
data = response.json()

weather = data[0]['IconPhrase']

temp = (float(data[0]['Temperature']['Value']) - 32) * (5/9)

# Print the contents of the response (the data the server returned)
print(str(temp) + "," + str(weather))

