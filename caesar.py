import cs50
import sys

def main():
    #only accepts two command line arguments

    if len(sys.argv) != 2:
        print("Error: This program pnly accepts one command line.")
        return 1
    else:
        #converts string to int and protects wraparound
        k = int(sys.argv[1]) % 26

        print("Plaintext: ")
        s = cs50.get_string()
        print("Ciphertext: ")

        #used key to change plaintext to ciphertext

        for i in range(len(s)):
            c = ord(s[i])

            for j in range(k):

                if (c >= 65 and c <= 90) or (c >= 97 and c <= 122):
                    if c == 90 or c == 122:
                        c = c - 26

                    c += 1
                else:
                    break
            print(chr(c), end="")

    print("")
    return 0



if __name__ == "__main__":
    main()