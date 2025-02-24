import base64
from http.client import responses

import requests


def main():

    while(True):
        path = input("Enter json file absolute path: ")

        if path == None:
            print("You must enter the path to a file.")
            break
        try:
            with open(path, "rb") as f:
                bytes = f.read()
            # encoded = base64.b64encode(bytes).decode('utf-8') // strings in python are saved as Unicode, UTF-8 is the default
            result = requests.post("http://localhost:5527", bytes)
            if result.status_code == 200:
                    print(result.content)
                    continue

        except ValueError:
            print("Path not recognized.\n")



if __name__ == '__main__':
    main()