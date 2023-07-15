import call_class_string
import sys

class user():
    name = ""
    word = ""

    def __init__(self, name, word):
        self.name = name
        self.word = word
args = ['darsh', 'hello']

print(*args)
print(getattr(call_class_string, 'user')(*args))


