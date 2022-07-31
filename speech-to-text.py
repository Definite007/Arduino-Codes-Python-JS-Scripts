import requests
upload_url = 'https://api.assemblyai.com/v2/upload'
endpoint = "https://api.assemblyai.com/v2/transcript"

headers = {'authorization': "Your Auth Key from Assembly AI"}
filename = "sample.wav"
def upload(filename):
    def read_file(filename, chunk_size=5242880):
        with open(filename, 'rb') as _file:
            while True:
                data = _file.read(chunk_size)
                if not data:
                    break
                yield data
    response = requests.post(upload_url,
                            headers=headers,
                            data=read_file(filename))
    audio_url = response.json()['upload_url']
    return audio_url

def transcribe(audio_url):
    json = { "audio_url": audio_url }
    response = requests.post(endpoint, json=json, headers=headers)
    job_id = response.json()['id']
    return job_id

def poll(trans_id):
    polling_endpoint = endpoint+ '/' + trans_id
    poll_res = requests.get(polling_endpoint, headers=headers)
    return poll_res.json()

def getresulturl(audio_url):
    trans_id = transcribe(audio_url)
    while True:
        data = poll(trans_id)
        if data['status'] == 'completed':
            return data, None
        elif data['status'] == 'error':
            return data, "error"

def save_transcript(audio_url):
    data, error = getresulturl(audio_url)
    if data:
        text_filename = filename[:-4] + '.txt'
        with open(text_filename, "w") as f:
            f.write(data['text'])
        print('Transcription done')
    elif error:
        print(error)

audio_url = upload(filename)
save_transcript(audio_url)
