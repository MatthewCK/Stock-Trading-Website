import cs50
import sys

def main ():
    print ("CC Number: ", end= "")
    n = cs50.get_int()
    m=n

    #gets digits in n
    count = 0
    while True:
        n /= 10
        count += 1
        if n < 10:
            count += 1
            break

    n=m
    #separates cc to array of ints
    digits = []
    arr1 = []
    arr2 = []

    for i in range(count):
        digits.append(int (n % 10))
        n /= 10

    #separates digits to every other 1
    for i in range(1, count, 2):
        arr1.append(digits[i])

    #separates digits to every other 2
    for i in range(0, count, 2):
        arr2.append(digits[i])

    for i in range(len(arr1)):
        arr1[i] = arr1[i] * 2

    s = 0

    #sums products' digits
    for i in range(len(arr1)):
        s = s + (int (arr1[i] % 10))
        arr1[i] = arr1[i] / 10
        s = s + (int (arr1[i] % 10))

    #sums with arr2
    for i in range(len(arr2)):
        s = s + arr2[i]

    digits.reverse()
    if int (s % 10) == 0:
        if count == 15:
            print("AMEX")

        elif count == 13:
            print("VISA")

        elif count == 16:

            a = str(digits[0]) + str(digits[1])

            if digits[0] == 4:
                print("VISA")

            elif a in ['51', '52', '53', '54', '55']:
                print("MASTERCARD")
            else:
                print("INVALID")
        else:
            print("INVALID")
    else:
        print("INVALID")

if __name__ == "__main__":
    main()