---
layout: post
date: 2018-05-07
title: Log base 10 vs Log base e notation
math: true
---

I wrote this post after struggling for a few hours with the derivation of the negative log-likelihood function , and finding out that the derivative didn't match what was expected, only to find out that it was indeed matching and that instead, there's a discrepancy on the semantics of the "**log**" notation.

Let's go step by step:

Two of the most common logarithms are the logarithm of base $$e$$, also called
natural logarithm which is written in two forms:

$$\ln{x} = \log_{e}{x}$$

and the logarithm of base 10 also called common logarithm or decimal
logarithm:

$$ \log_{10}{x}$$

Additionally scientific calculators have a "**log**" and a  "**ln**" function,
which apply the logarithm of base 10 and the logarithm of base $$e$$ respectively:

![log keys in calculator](https://upload.wikimedia.org/wikipedia/commons/8/88/Logarithm_keys.jpg)

##### By Waifer X - 100502-1150494Uploaded by Pieter Kuiper, CC BY 2.0, https://commons.wikimedia.org/w/index.php?curid=10715338

But something that I learned while checking for errors doing the derivative is that mathematicians and calculator designers
have different things in mind when writing "**log**":

From Wikipedia's [Common Logarithm](https://en.wikipedia.org/wiki/Common_logarithm):

> On calculators it is usually "log", but mathematicians usually mean natural logarithm (logarithm with base e ≈ 2.71828) rather than common logarithm when they write "log". To mitigate this ambiguity the ISO 80000 specification recommends that $$log_{10}{x}$$ should be written $$lg{x}$$ and $$log_{e}{x}$$ should be $$ln{x}$$.

And from Wikipedia's [Natural Logarithm](https://en.wikipedia.org/wiki/Natural_logarithm):

> The natural logarithm of x is generally written as $$ln x$$, $$log_{e} x$$, or sometimes, if the base $$e$$ is implicit, simply $$log x$$.

Therefore in the case of the negative log-likelihood function:

$$ L(\boldsymbol{y}) = - \log(\boldsymbol{y}) $$

Although it's not implicit that it refers to a logarithm of base $$e$$, *it is
indeed the natural logarithm*, it's just being used as mentioned in the given
excerpt of common logarithm (i.e: mathematicians usually mean natural logarithm when they write "log"), and therefore its derivative is:

$$ \dfrac{\partial L_{\boldsymbol{y}}}{\partial \boldsymbol{y}} = -\dfrac{1}{\boldsymbol{y}} $$


Mathematicians be crazy!
