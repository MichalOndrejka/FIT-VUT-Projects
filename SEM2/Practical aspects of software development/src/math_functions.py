#Implement math functions here
def addition(addend1, addend2):
    """Returns addition of addend1 and addend2
    \n(addend1 + addend2)"""
    return float(addend1 + addend2)

def subtract(minuend, subtrahend):
    """Returns subtraction of minuend and subtrahend
    \n(minuend - subtrahend)"""
    return float(minuend - subtrahend)

def multiply(multiplicand, multiplayer):
    """Returns multiplication of multiplicand and multiplayer
    \n(multiplicand * multiplayer)"""
    return float(multiplicand * multiplayer)

def divide(divident, divisor):
    """Returns division of divident and divisor
    \n(divident / divisor)
    \nDivisien by zero prints error on stderr"""
    if (divisor == 0):
        raise ValueError("Division by zero is not defined")
    return float(divident / divisor)    

def exponentiate(base, exponent):
    """Returns power of base and exponent
    \n(base ** exponent)"""
    if exponent < 0:
        raise ValueError("Exponent lower than 0")
    return float(base ** exponent)

def nthroot(radicand, base):
    """Returns nth root of base and radicand
    \n(base ** (1/radicand))
    \nAny root from base < 0 prints error on stderr"""
    if (base < 0):
        raise ValueError("Base is lower than 0")
    if (radicand <= 0):
        raise ValueError("Radicand is lower or equal 0 ")
    #Result must be one decimal place shorter, so exponent could be periodic.
    return float(round((base ** (1/radicand)),15))

def factorial(number):
    """Returns factorial of number
    \n(number!)
    \nAny factorial from number < 0 prints error on stderr"""
    if float(number) % 1 != 0:
        raise ValueError("Integer number expected")
    if number < 0:
        raise ValueError("Factorial exists for positive numbers only")
    if number == 0:
        return 1

    if number == 1:
        return number
    return float(number*factorial(number-1))

def absolute(number):
    """Returns absolute value of number
    \n(|number|)"""
    if number < 0:
        return float(-number)
    if number >= 0:
        return float(number)