import addressbook_pb2
import sys

def PromptForAddress(person):
    person.id = int(raw_input("Please input the id of this person..."))
    person.name = raw_input("Please input a name for the person...")
    email = raw_input("Please enter the email address of the person....")

    if email != "":
        person.email = email
        pass

    while True:
        number = raw_input("Enter a phone number :")
        if number == "":
            break
            pass

        phone_number = person.phone.add()
        phone_number.number = number


        type = raw_input("Is this a mobile, home, or work phone?")
        if type == "mobile":
            phone_number.type = addressbook_pb2.Person.MOBILE
        elif type == "home":
            phone_number.type = addressbook_pb2.Person.HOME
        elif type == "work":
            phone_number.type = addressbook_pb2.Person.WORK
        else:
            print("Unknown phont type; leaving as default value.")
            pass
        pass
    pass


if len(sys.argv) != 2:
    print("Usage:", sys.argv[0], "ADDRESS_BOOK_FILE")
    sys.exit(-1)
    pass
print("What am i doing....")
address_book = addressbook_pb2.AddressBook()

try:
    f = open(sys.argv[1], "rb")
    address_book.ParseFromString(f.read())
    f.close()
except IOError, e:
    print(sys.argv[1] + " : File not found. Creating a new file")
    pass

PromptForAddress(address_book.person.add())

f = open(sys.argv[1], "wb")
f.write(address_book.SerializeToString())
f.close()

add_person.py
