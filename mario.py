import cs50

def main():
    while True:
        print ("Height: ", end="")
        b = cs50.get_int()
        if b > -1 and b < 24:
            break
        
    for i in range(b):
        s = b - 1 - i
        h = b - s
        
        for x in range(s):
            print (" ", end="")
        for x in range(h):
            print ("#", end="")
        
        print ("  ", end="")
        
        for x in range(h):
            print ("#", end="")
            
        print ("")
        
if __name__ == "__main__":
    main()