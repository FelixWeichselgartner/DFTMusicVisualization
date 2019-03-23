from math import cos, pi

def vonHann(alpha, beta, N):
    """implementation of the vonHann window function.
    
    Arguments:
        alpha {double} -- alpha value
        beta {double} -- beta value
        N {integer} -- length of the function
    
    Returns:
        list -- list containing window function with the length N
    """

    vonHann = [None] * N
    for n in range(N):
        vonHann[n] = alpha - beta * cos(2 * pi * n / (N - 1))
    return vonHann

def hamming(N):
    """implementation of the hamming window function.
    
    Arguments:
        N {integer} -- length of the function
    
    Returns:
        list -- list containing window function with the length N
    """

    alpha = 25.0/46
    beta = 1 - alpha
    return vonHann(alpha, beta , N)

from plot import singlePlot

def main():
    test = hamming(16)
    singlePlot(test)
    #mulitiply hamming(length(signal)) * signal

if __name__ == "__main__":
    main()