import pyrebase
import pandas as pd

firebaseConfig = {
    "apiKey": "AIzaSyBEHvCZQpcogdq1o_9BEPW3xgh9Tq0vdB4",
    "authDomain": "cyclic-voltammetry-fe847.firebaseapp.com",
    "databaseURL": "https://cyclic-voltammetry-fe847-default-rtdb.firebaseio.com",
    "projectId": "cyclic-voltammetry-fe847",
    "storageBucket": "cyclic-voltammetry-fe847.appspot.com",
    "messagingSenderId": "183536016835",
    "appId": "1:183536016835:web:cdec2eb19b14ed6b8e7176"
}

firebase=pyrebase.initialize_app(firebaseConfig)

db=firebase.database()

result_voltage = db.child("coletas").child("voltage").get()
result_current = db.child("coletas").child("current").get()

voltage_data = []
current_data = []

for value in result_voltage.each():
    voltage_data.append(value.val())

for value in result_current.each():
    current_data.append(value.val())

df = pd.DataFrame({'Coleta1-tensão':voltage_data,
                   'Coleta1-corrente':current_data})
                  
df.to_csv('voltametria.csv')