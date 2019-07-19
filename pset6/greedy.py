import cs50

while True:
    print("O hai! How much change is owed?\n")
    change = cs50.get_float()
    if change>0:
        break

cents = int(round(change*100))
count = 0;
while cents>0:
    if cents>=25:
        count = count+1
        cents = cents-25
    elif cents>= 10:
        cents = cents-10
        count = count+1
    elif cents>= 5:
        cents = cents-5
        count = count+1
    elif cents>= 1:
        cents = cents-1
        count = count+1

print(count)
