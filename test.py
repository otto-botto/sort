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
               "items": ["cat", "dog", "mouse"]
           })
        self.assertEqual(200, result.status_code)
        self.assertEqual(result.request.method, "POST")
        self.assertEqual(result.content, b"Request successfully sent.\n")


    def test_collect_info(self):
        dict_returned = collect_info()
        self.assertEqual(dict_returned, ["/home/lora/Documents/books/otto", "otto", "botto", "plk", 1, 1])

    def test_send_post_request(self):
        result = client.collect_info()
        self.assertEqual(result.content, b"Book was added to the server.\n")

class TestListingFiles(unittest.TestCase):
    def test_list_files(self):
        result = requests.get("http://localhost:5525/list")
        self.assertEqual(200, result.status_code)
        self.assertEqual("GET", result.request.method)
        self.assertEqual(b"otto\ntiny_c_projects.pdf\nalgorithmic_thinking\n", result.content)

if __name__ == '__main__':
    unittest.main()