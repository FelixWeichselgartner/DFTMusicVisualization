def zeroPadding(function, n):
    """adds zeros to a list -> zero padding to increase frequency resolution.
    
    Arguments:
        function {list} -- function discrete in a list
        n {integer} -- amount of zeros to append
    
    Returns:
        list -- function with the added zeros
    """

    for x in range(n):
        function.append(0)
    return function

def zeroPaddingToNextPOW2N(function):
    """adds the amount of zeros to get len(function) to be pow(2, n), with n being the next higher power of 2.
    
    Arguments:
        function {list} -- function discrete in a list
        
    Returns:
        list -- function with the added zeros
    """

    length = len(function)
    n = 1
    while length > pow(2, n):
        n += 1 
    #print(f'n = {n}')
    add = pow(2, n) - length
    #print(f'length {length} + add {add} = {length + add} == {pow(2, n)}')
    function = zeroPadding(function, add)
    return function
    
from math import pi

def main():
    #signal x
    x = []
    #making a dc signal with 5V here
    amount = 10
    n = 0
    inkrement = 2*pi/amount
    i = 0
    for i in range(amount):
        x.append(5)
        n = n + inkrement

    N = len(x)

    #original signal
    print(f'signal: x = {x}\n\n')

    x = zeroPaddingToNextPOW2N(x)

    #after zero padding
    print(f'signal: x = {x}\n\n')

if __name__ == "__main__":
    main()