## **Implementing the Fast Fourier Transform**

The core aspect of my project is to perform spectrum analysis on an analogue voltage waveform digitised by an ADC. To do this I have implemented the Fast Fourier Transform (FFT).

The FFT algorithm is an specific optimisation of the Discrete Fourier Transform (DFT), the mathematical method for determining the frequency content of a discrete set of samples of a signal. A naive algorithmic implementation of the DFT, as it is defined, would have a computation time of
<img src="https://render.githubusercontent.com/render/math?math={ O(N^2)}#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\color{white} O(N^2)}#gh-dark-mode-only">
, meaning the time it would take to complete is proportional to the square of the number of samples (N). The FFT has a computation time of 
<img src="https://render.githubusercontent.com/render/math?math={ O(N \log_2 N )}#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\color{white} O(N \log_2 N )}#gh-dark-mode-only">
, a vast improvement over the DFT, especially for large N. Here's how it works.

Consider a sequency of samples "s" of length N, where any particular samples is found using an index into the sequency, as s[n]. The Discrete Fourier Transform produces an output sequence S of the same length where each of the entries in the sequence are frequency components. Therefore, for any frequency f the DFT is defined as

<br/>
<div style = "text-align:center">
<img src="https://render.githubusercontent.com/render/math?math={\large S[f] = \displaystyle\sum_{n = 0}^{N-1} {s[n] (e^{-2 \pi j {n \over N}})^f}}#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\large \color{white}S[f] = \displaystyle\sum_{n = 0}^{N-1} {s[n] (e^{-2 \pi j {n \over N}})^f}}#gh-dark-mode-only">
</div>
<br/>

This is a sum over all the samples in the input sequency, multiplied by a complex exponential coefficient, at a particular frequency.

To understand what the DFT is doing Consider a series of samples of a cosine wave:
PICTURE

If the samples are viewed on the complex plane, multiplying the value of each sample by 
<img src="https://render.githubusercontent.com/render/math?math={ e^{-2 \pi j {n \over N}}}#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\color{white} e^{-2 \pi j {n \over N}}}#gh-dark-mode-only">
rotates it anticlockwise through an angle that is equivalent to it's position in the sample sequence. PICTURE

Doing this for each sample spreads the samples out with equal spacing around the unit circle on the complex plane. PICTURE

The sum is the same as adding up all of these vectors and finding their end point. PICTURE

the value of $f$ in the DFT represents the number of rotations through wich the points are wrapped around the complex plane. For instance if f = 1 the points will be spread evenly over one revolution, if f = 2 the points will be spread evenly over two revolutions (when f = 0 all of the vectors are simply added together end to end with no rotation, this represents the DC component of the signal).

$f$ can be seen as the frequency component of the signal currently being analysed. The number of times the points are wrapped around will change the ultimate value of the sum, the result of which represents "how much" of the signal is present at that particular frequency.




**Back to the explanation of the FFT**. It turns out that the DFT sum is exactly equivalent to first taking the sum of all the even terms in the sequence, then the sum of all the odd terms and then adding these sums together.

<br/>
<div style = "text-align:center">
<img src="https://render.githubusercontent.com/render/math?math={\large S[f] = \displaystyle\sum_{m = 0}^{{N \over 2}-1}{s[2m] (e^{-2 \pi j \frac{2m}{N}})^f} %2B \displaystyle\sum_{m = 0}^{{N \over 2}-1} {s[2m %2B 1] (e^{-2 \pi j \frac{2m %2B 1}{N}})^f}}#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\large \color{white} S[f] = \displaystyle\sum_{m = 0}^{{N \over 2}-1}{s[2m] (e^{-2 \pi j \frac{2m}{N}})^f} %2B \displaystyle\sum_{m = 0}^{{N \over 2}-1} {s[2m %2B 1] (e^{-2 \pi j \frac{2m %2B 1}{N}})^f}}#gh-dark-mode-only">
</div>
<br/>

expanding the exponent of e gives

<br/>
<div style = "text-align:center">
<img src="https://render.githubusercontent.com/render/math?math={\large S[f] = \displaystyle\sum_{m = 0}^{{N \over 2}-1} {s[2m] (e^{-2 \pi j \frac{2m}{N}})^f} %2B \displaystyle\sum_{m = 0}^{{N \over 2}-1} {s[2m %2B 1] (e^{-2 \pi j \frac{2m}{N}}e^{-2 \pi j {{1 \over N}}})^f}}#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\large \color{white} S[f] = \displaystyle\sum_{m = 0}^{{N \over 2}-1} {s[2m] (e^{-2 \pi j \frac{2m}{N}})^f} %2B \displaystyle\sum_{m = 0}^{{N \over 2}-1} {s[2m %2B 1] (e^{-2 \pi j \frac{2m}{N}}e^{-2 \pi j {{1 \over N}}})^f}}#gh-dark-mode-only">
</div>
<br/>

and multiplying f into one of the exponents

<br/>
<div style = "text-align:center">
<img src="https://render.githubusercontent.com/render/math?math={\large S[f] = \displaystyle\sum_{m = 0}^{{N \over 2}-1} {s[2m] (e^{-2 \pi j \frac{2m}{N}})^f} %2B \displaystyle\sum_{m = 0}^{{N \over 2}-1} {s[2m %2B 1] (e^{-2 \pi j \frac{2m}{N}})^f e^{-2 \pi j {{f \over N}}}}}#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\large \color{white} S[f] = \displaystyle\sum_{m = 0}^{{N \over 2}-1} {s[2m] (e^{-2 \pi j \frac{2m}{N}})^f} %2B \displaystyle\sum_{m = 0}^{{N \over 2}-1} {s[2m %2B 1] (e^{-2 \pi j \frac{2m}{N}})^f e^{-2 \pi j {{f \over N}}}}}#gh-dark-mode-only">
</div>
<br/>

The second sum now has a multiplicative factor that no longer depends on the particular entry in the sequence, m, so it can be factorised out of the sum.

<br/>
<div style = "text-align:center">
<img src="https://render.githubusercontent.com/render/math?math={\large S[f] = \displaystyle\sum_{m = 0}^{{N \over 2}-1} {s[2m] (e^{-2 \pi j \frac{2m}{N}})^f} %2B e^{-2 \pi j {{f \over N}}} \displaystyle\sum_{m = 0}^{{N \over 2}-1} {s[2m %2B 1] (e^{-2 \pi j \frac{2m}{N}})^f}}#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\large \color{white} S[f] = \displaystyle\sum_{m = 0}^{{N \over 2}-1} {s[2m] (e^{-2 \pi j \frac{2m}{N}})^f} %2B e^{-2 \pi j {{f \over N}}} \displaystyle\sum_{m = 0}^{{N \over 2}-1} {s[2m %2B 1] (e^{-2 \pi j \frac{2m}{N}})^f}}#gh-dark-mode-only">
</div>
<br/>

Comparing with the original definition of the DFT it is now clear the two sums in the equation above are themselves simply the DFT of all the even terms and the DFT of all the odd terms

<br/>
<div style = "text-align:center">
<img src="https://render.githubusercontent.com/render/math?math={\large S[f] = DFT(2m) %2B e^{-2 \pi j {{f \over N}}} DFT(2m %2B 1)}#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\large \color{white} S[f] = DFT(2m) %2B e^{-2 \pi j {{f \over N}}} DFT(2m %2B 1)}#gh-dark-mode-only">
</div>
<br/>

These DFT's can continue to be broken down recursively using the same process until all that is left is pairs of samples from the input sequence. 

This recursion is neat but it is not actually where the speed improvement of the FFT comes from. This is just a recursive expression of the DFT. The important fact is that the even and odd sub-sequences have twice the periodicity of the original sequence, meaning the value of the even and odd sums will repeat as f goes from 0 to N - 1. Computationally it would be a waste to calculate these repeated values more than once.

As the recursion descends the values of the remaining sums repeat more and more often with increasing f, until at the very bottom of the recursion the result of the DFT simply alternates back and fourth between two values as f increases. This can be shown by taking the DFT of a pair of samples (N = 2)

<br/>
<div style = "text-align:center">
<img src="https://render.githubusercontent.com/render/math?math={\large S[f] = \displaystyle\sum_{n = 0}^{N-1} {s[n] (e^{-2 \pi j {n \over N}})^f} = \displaystyle\sum_{n = 0}^{1} {s[n] (e^{-2 \pi j {n \over 2}})^f} = s[0](e^{-2 \pi j {0 \over 2}})^f %2B s[1](e^{-2 \pi j {1 \over 2}})^f = s[0] %2B s[1]e^{-\pi j f}}#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\large \color{white} S[f] = \displaystyle\sum_{n = 0}^{N-1} {s[n] (e^{-2 \pi j {n \over N}})^f} = \displaystyle\sum_{n = 0}^{1} {s[n] (e^{-2 \pi j {n \over 2}})^f} = s[0](e^{-2 \pi j {0 \over 2}})^f %2B s[1](e^{-2 \pi j {1 \over 2}})^f = s[0] %2B s[1]e^{-\pi j f}}#gh-dark-mode-only">
</div>
<br/>

As f is increased in integer steps from 0 the value of 
<img src="https://render.githubusercontent.com/render/math?math={ e^{-\pi j f}}#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\color{white} e^{-\pi j f}}#gh-dark-mode-only">
toggles between 1 and -1. This is because 
<img src="https://render.githubusercontent.com/render/math?math={ e^{j \pi} = -1}#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\color{white} e^{j \pi} = -1}#gh-dark-mode-only">
and is periodic with <img src="https://render.githubusercontent.com/render/math?math={ 2\pi}#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\color{white} 2\pi}#gh-dark-mode-only">
.

So the only unique values of the sum down here at the bottom of the recursion are s[0] + s[1] and s[0] - s[1]. (Note that f still varies from 0 to whatever the original length, N, of the full sequence was). The naive recursive method would wastefully calculate these two results over and over again. Instead, for the same number of steps we could calculate these two unique results for all of the other pairs of samples from the input sequence, i.e. s[2] + s[3] and s[2] - s[3], s[4] + s[5] and s[4] - s[5], s[6] + s[7] and s[6] - s[7] and so on...

*on a side note my use of sequential indices here is a bit missleding. Since these pairs represent the even and odd subsets of sequences that themselves are the even and odd subsets of other sequences the indices will be shuffled around quite a bit. Here's an example for a sequence of 8 samples:*

<br/>
<div style = "text-align:center">
<img src="https://render.githubusercontent.com/render/math?math={\large s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7]}#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\large \color{white} s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7]}#gh-dark-mode-only">
</div>
<br/>
<div style = "text-align:center">
<img src="https://render.githubusercontent.com/render/math?math={\large even:\ s[0], s[2], s[4], s[6]\ \ \ \ \ odd:\ s[1], s[3], s[5], s[7]}#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\large \color{white} even:\ s[0], s[2], s[4], s[6]\ \ \ \ \ odd:\ s[1], s[3], s[5], s[7]}#gh-dark-mode-only">
</div>
<br/>
<div style = "text-align:center">
<img src="https://render.githubusercontent.com/render/math?math={\large even:\ s[0], s[4]\ \ \ \ \ odd:\  s[2], s[6]\ \ \ \ \ even:\ s[1], s[5]\ \ \ \ \ odd:\ s[3], s[7]}#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\large \color{white} even:\ s[0], s[4]\ \ \ \ \ odd:\  s[2], s[6]\ \ \ \ \ even:\ s[1], s[5]\ \ \ \ \ odd:\ s[3], s[7]}#gh-dark-mode-only">
</div>
<br/>
The resulting shuffled sequence is:

</br>
<div style = "text-align:center">
<img src="https://render.githubusercontent.com/render/math?math={\large shuffled\ sequence = s[0], s[4], s[2], s[6], s[1], s[5], s[3], s[7]}#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\large \color{white} s[0], s[4], s[2], s[6], s[1], s[5], s[3], s[7]}#gh-dark-mode-only">
</div>
<br/>

The periodicity in the results of the DFT in f can be exploited at each level up the recursion until the very top. Though hopefully the explanation above provides some insight the process is rather difficult to put into words. Helpfully it can be visulaised using a "butterfly diagram". The following butterfly diagram shows the stages of computing the FFT for an input sequence of 16 samples. At each stage the frequency components are calculated as the sum of the even component from the previous stage and a coefficient 
<img src="https://render.githubusercontent.com/render/math?math={e^{-2 \pi j {{f \over N}}}}#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\color{white} e^{-2 \pi j {{f \over N}}}}#gh-dark-mode-only"> multiplied by the odd component. PICTURE

### **Implementation details**




<img src="https://render.githubusercontent.com/render/math?math={ }#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\color{white} }#gh-dark-mode-only">

<div style = "text-align:center">
<img src="https://render.githubusercontent.com/render/math?math={\large }#gh-light-mode-only">
<img src="https://render.githubusercontent.com/render/math?math={\large \color{white} }#gh-dark-mode-only">
</div>