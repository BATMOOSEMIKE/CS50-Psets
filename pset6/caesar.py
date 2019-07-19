import cs50
import sys

if len(sys.argv) != 2:
    print("Sorry, that's not a valid input")
    exit(1)

k = int(sys.argv[1])
print("plaintext: ")
text = cs50.get_string()
cipher = []
print("ciphertext: ")

for letter in text:
    if letter.isupper():
        cipher.append (chr(((ord(letter) - 65 + k) % 26) + 65))
    elif letter.islower:
        cipher.append(chr(((ord(letter) - 97 + k) % 26) + 97))
    else: 
        cipher.append(letter)
        
print("".join(cipher))

