import base64
import unittest
import client
import requests
import os



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


class TestSavingFiles(unittest.TestCase):
    def test_send_json_strings(self):
        path = "/home/lora/Documents/json/animals.json"
        with open(path, "rb") as f:
            bytes = f.read()
            # encoded = base64.b64encode(bytes).decode('utf-8') // strings in python are saved as Unicode, UTF-8 is the default
        result = requests.post("http://localhost:5527", bytes)
        self.assertEqual(200, result.status_code)
        self.assertEqual(result.request.method, "POST")
        self.assertEqual(result.content, b"{\"strings\" : [\"cat\", \"dog\", \"mouse\", \"squirrel\"]}")

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