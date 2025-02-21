import base64
from http.client import responses

import requests

def collect_info():
    last = input("Enter author's last name: ").lower()
    first = input("Enter author's first name: ").lower()
    title = input("Enter title, spaces are _: ").lower()

    # accept only digits from 1 to 5
    while True:
        try:
            genreNum = int(input("Select genre (enter digit): [1] FICTION [2] NONFICTION [3] MYSTERY [4] SCIFI [5] CHILDREN: "))
            if 1<= genreNum <= 5:
                break
            else:
                print("Invalid input. Please select a number between 1 and 5.")
        except ValueError:
            print("Invalid input. Please select a number.")

    path = input("Enter absolute path: ")

    # accept only digits from 1 to 3
    while True:
        try:
            formatNum = int(input("Select file type (enter digit): [1] txt [2] pdf [3] epub: "))
            if 1<= formatNum <= 3:
                break
            else:
                print("Invalid input. Please select a number between 1 and 3.")
        except ValueError:
            print("Invalid input. Please select a number.")
    # comment out response for testing
    response = send_post_request(path, last, first, title, genreNum, formatNum)
    # return for testing
    # return [path, first, last, title, genreNum, formatNum]

    return response
def send_post_request(path, last, first, title, genreNum, formatNum):
    format = ""
    genre = ""

    # set genre
    match genreNum:
        case 1:
            genre = "FICTION"
        case 2:
            genre = "NONFICTION"
        case 3:
            genre = "MYSTERY"
        case 4:
            genre = "SCIFI"
        case 5:
            genre = "CHILDREN"

    # set file format
    match formatNum:
        case 1:
            format = "txt"
        case 2:
            format = "pdf"
        case 3:
            format = "epub"

    with open(path, "rb") as f:
        bytes = f.read()
    encoded = base64.b64encode(bytes).decode('utf-8')


    return requests.post("http://localhost:5525", json={
        "last": last,
        "first": first,
        "title": title,
        "genre": genre,
        "format": format,
        "length": len(encoded),
        "file": encoded
    })

def list_files():
    request = requests.get("http://localhost:5525/list")
    return request

def main():
    request = requests.get("http://localhost:5525/")
    if request.status_code == 200:
        print("Welcome to the files server. You have successfully connected.\n"
              "On this server, you can store book files and retrieve them by author or genre."
              "Can't remember the author? Search by date added.\n")
    print("Type a command \n"
          "[add] to add a file\n"
          "[list] to list all files\n"
          "[logout] to exit server:\n"
          ">>> ")


    while(True):
        command = input("Enter command: ")

        if command == "logout":
            print("You have successfully logged out. Goodbye.")
            break
        try:
            if command == "add":
                response = collect_info()
                if response.status_code == 200:
                    print("title successfully added to the server")
                    continue
            elif command == "list":
                response = list_files()
                if response.status_code == 200:
                    print("Listing all files: \n")
                    print(response.content)
                    continue
        except ValueError:
            print("Command not recognized.\n")



if __name__ == '__main__':
    main()