import base64
# from http.client import responses
import requests
import os
import requests


def main():
    resultGet = requests.get("http://localhost:5527")
    if resultGet.status_code == 200:
        print("Connected to sorting server.")
    while(True):

        command = input("To send a file to be sorted, type <send>. To quit, type <quit>: ")

        if command == "quit":
            break

        path = input("Enter json file absolute path: ")

        if path == None:
            print("You must enter the path to a file.")
            break
        try:
            with open(path, "rb") as f:
                bytes = f.read()
                print("Sending bytes: ")
                print(bytes)
            # encoded = base64.b64encode(bytes).decode('utf-8') // strings in python are saved as Unicode, UTF-8 is the default
            result = requests.post("http://localhost:5527", bytes)
            if result.status_code == 200:
                print("received bytes: ")
                print(result.content)
                continue

        except FileNotFoundError:
            print("Path not recognized.\n")



if __name__ == '__main__':
    main()