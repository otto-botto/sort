import base64
import unittest
import client
import requests
import os

from client import collect_info


class TestGettingFiles(unittest.TestCase):
    def test_getting_file_size(self):
        path = "/home/lora/Documents/books/Tiny_C_Projects.pdf"
        file_size = os.path.getsize(path)
        with open(path, "rb") as f:
            bytes = f.read()
        encoded = base64.b64encode(bytes).decode('utf-8')
        print(len(encoded))
        # print(bytes)
        print(file_size)


    def test_landing_page(self):
        result = requests.get("http://localhost:5525/")
        self.assertEqual(200, result.status_code)
        self.assertEqual("GET", result.request.method)
        self.assertEqual(b"Welcome to the files server.\n", result.content)

    def test_cat_image(self):
        result = requests.get("http://localhost:5525/cat.jpg")
        self.assertEqual(200, result.status_code)
        self.assertEqual(41006, len(result.content))
        self.assertEqual("image/jpeg", result.headers['content-type'])


class TestSavingFiles(unittest.TestCase):
    def test_add_file_reaching(self):
        result = requests.post("http://localhost:5527/")
        self.assertEqual(result.status_code, 200)
        self.assertEqual(result.request.method, "POST")
        self.assertEqual(result.content, b"Request successfully sent.\n")

    def test_send_json_numbers(self):
        result = requests.post("http://localhost:5527",
            json={
            "type": "NUMBER",
            "items": [100, 20, 15]
        })
        self.assertEqual(200, result.status_code)
        self.assertEqual(result.request.method, "POST")
        self.assertEqual(result.content, b"Request successfully sent.\n")

    def test_send_json_alpha(self):
        result = requests.post("http://localhost:5527",
           json={
               "type": "ALPHA",
               "items": ["squirrel", "dog","cat", "mouse"]
           })
        self.assertEqual(200, result.status_code)
        self.assertEqual(result.request.method, "POST")
        self.assertEqual(result.content, b"{\"strings\" : [\"cat\", \"dog\", \"mouse\", \"squirrel\"]}")

    def test_send_json_dates(self):
        result = requests.post("http://localhost:5527",
                               json={
                                   "type": "CHRONO",
                                   "items": ["1986-04-19", "1986-04-15", "2004-11-23", "2024-12-31"]
                               })
        self.assertEqual(200, result.status_code)
        self.assertEqual(result.request.method, "POST")
        self.assertEqual(result.content, b"{\"strings\" : [\"1986-04-15\", \"1986-04-19\", \"2004-11-23\""
                                         b", \"2024-12-31\"]}")




if __name__ == '__main__':
    unittest.main()