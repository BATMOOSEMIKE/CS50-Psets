import cs50

while True:
    print("Choose the height (between 0 and 23): ", end="")
    height = cs50.get_int()
    if(height>0 and height<23):
        break
for i in range(height):
    for x in range(height-i):
        print(" ", end = "")
    for a in range(i+1):
        print("#", end = "")
    print("")
