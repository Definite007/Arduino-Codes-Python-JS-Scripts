from datetime import datetime
import os
import webbrowser
import pyttsx3
import speech_recognition as sr
import wikipedia

engine = pyttsx3.init('sapi5')
voices = engine.getProperty('voices')
engine.setProperty('voice', voices[0].id)

def speak(audio):
    engine.say(audio)
    engine.runAndWait()

def wishme():
    hour = int(datetime.now().hour)
    if hour >= 0 and hour < 12:
        speak("Good Morning")
    elif hour >= 12 and hour < 18:
        speak("Good Afternoon")
    else:
        speak("Good Evening")
    speak("Sir My name is Jarvis, PLease tell me how may I help you?")

def takeCommand():
    r = sr.Recognizer()
    with sr.Microphone() as source:
        print("Listening")
        r.energy_threshold = 200
        r.non_speaking_duration = 1
        audio = r.listen(source)
    try:
        print("Recognizing")
        query = r.recognize_google(audio, language='en-in')
    except Exception as e:
        print(e)
        print("Say that again please")
        speak("Say that again please")
        return "None"
    return query

wishme()

while True:
    query = takeCommand().lower()
    if 'wikipedia' in query:
        speak("Searching through Wikipedia")
        query = query.replace("wikipedia", "")
        results = wikipedia.summary(query, sentences=2)
        speak("According to Wikipedia...")
        print("According to Wikipedia...", results)
        speak(results)
    elif 'open youtube' in query:
        webbrowser.open("youtube.com")
    elif 'open google' in query:
        webbrowser.open("google.com")
    elif 'open stackoverflow' in query:
        webbrowser.open("stackoverflow.com")
    elif 'time' in query:
        time1 = datetime.now().strftime('%H:%M:%S')
        speak("Sir the time is")
        speak(time1)
    elif 'open code' in query:
            code_path = "C:\\Users\user\\AppData\Local\\Programs\\Microsoft VS Code\\Code.exe"
            os.startfile(code_path)
    
    


