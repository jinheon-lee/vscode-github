import os
import sys
import urllib.request

client_id = "2LVTWditn3W0FB45_JiP"
client_secret = "8A7L6PvTjG"
a=input("번역할 문장을 입력하세요.")
encText = urllib.parse.quote(a)
data = "source=ko&target=en&text=" + encText
url = "https://openapi.naver.com/v1/papago/n2mt"
request = urllib.request.Request(url)
request.add_header("X-Naver-Client-Id",client_id)
request.add_header("X-Naver-Client-Secret",client_secret)
response = urllib.request.urlopen(request, data=data.encode("utf-8"))
rescode = response.getcode()
if(rescode==200):
    response_body = response.read()
    print(response_body.decode('utf-8')
elif true:
    print("Error Code:" + rescode)
