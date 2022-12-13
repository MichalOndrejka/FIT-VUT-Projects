import unittest
import math_functions as calculator

class test_calc(unittest.TestCase):
    def test_addition(self):
        self.assertEqual(calculator.addition(20,15),35)
        self.assertEqual(calculator.addition(0,0),0)
        self.assertEqual(calculator.addition(-1,2),1)
        self.assertEqual(calculator.addition(-1,-1),-2)
        self.assertEqual(calculator.addition(1.5,0.25),1.75)

    def test_subtract(self):
        self.assertEqual(calculator.subtract(20,15),5)
        self.assertEqual(calculator.subtract(0,0),0)
        self.assertEqual(calculator.subtract(-5,8),-13)
        self.assertEqual(calculator.subtract(-1,-1),0)
        self.assertEqual(calculator.subtract(1.5,0.25),1.25)
    
    def test_multiply(self):
        self.assertEqual(calculator.multiply(2,15),30)
        self.assertEqual(calculator.multiply(0,8),0)
        self.assertEqual(calculator.multiply(-1,2),-2)
        self.assertEqual(calculator.multiply(-10,-10),100)
        self.assertEqual(calculator.multiply(1.5,0.25),0.375)
    
    def test_divide(self):
        self.assertEqual(calculator.divide(100,20),5)
        self.assertEqual(calculator.divide(10,4),2.5)
        self.assertEqual(calculator.divide(-10,-2),5)
        self.assertEqual(calculator.divide(9,-3),-3)
        self.assertRaises(ValueError,calculator.divide, 5,0)
        self.assertEqual(calculator.divide(1.5,0.25),6)

    def test_exponentiate(self):
        self.assertEqual(calculator.exponentiate(10,2),100)
        self.assertEqual(calculator.exponentiate(2,10),1024)      
        self.assertEqual(calculator.exponentiate(-4,2),16)
        self.assertEqual(calculator.exponentiate(-4,3),-64)
        self.assertEqual(calculator.exponentiate(100,0),1)
        self.assertEqual(calculator.exponentiate(0.25,1.5),0.25**1.5)

    def test_nthroot(self):
        self.assertEqual(calculator.nthroot(2,100),10)
        self.assertEqual(calculator.nthroot(3,64),4)
        self.assertRaises(ValueError,calculator.nthroot, 2,-4)
        self.assertEqual(calculator.nthroot(3,8),2)
        self.assertEqual(calculator.nthroot(2,0),0)
        self.assertEqual(calculator.nthroot(2,70.5),70.5**(1/2))


    def test_factorial(self):
        self.assertEqual(calculator.factorial(7),5040)
        self.assertRaises(ValueError,calculator.factorial, -7)
        self.assertEqual(calculator.factorial(1),1)
        self.assertEqual(calculator.factorial(0),1)
        self.assertRaises(ValueError,calculator.factorial, 5.1)

    def test_absolute(self):
        self.assertEqual(calculator.absolute(10),10)
        self.assertEqual(calculator.absolute(-50),50)
        self.assertEqual(calculator.absolute(0),0)
        self.assertEqual(calculator.absolute(3.14),3.14)
        self.assertEqual(calculator.absolute(-11.5),11.5)


if __name__ == '__main__':
    unittest.main()