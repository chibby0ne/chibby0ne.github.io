---
layout: post
date: 2018-05-11
title: Installing Tensorflow with GPU acceleration in Arch
math: true
---

I recently started doing the [fast.ai](https://www.fast.ai) course where you follow a top-down
approach into the world of machine learning.

The first thing you need to do is have access to a Nvidia GPU, and in the
first video lesson they show the setup of a cloud instance that runs
Tensorflow using either [crestle](https://www.crestle.com) or
[paperspace](https://www.paperspace.com).

But since either of them are paid by the hour, and considering I already have
Nvidia GPU in my desktop (albeit not as powerful as the ones offered in the
aforementioned services, just an GTX 1070), I thought of going in a tangent
and setting it up myself.


The first thing to do is install Tensorflow.

I already had CUDA installed, so I proceeded to install Tensorflow.

There was a problem: the CUDA version was too bleeding edge for the Tensroflow
version. Tensorflow 1.9 requires CUDA 9.0 and the latest CUDA in Arch
repositories was 9.1.

I thought of compiling Tensorflow myself, since that would make it work with
9.1. But that would take a long time and probably space as well.

Instead I opted out for a docker image which required [nvidia-docker](https://aur.archlinux.org/packages/nvidia-docker/) from AUR,
and rebooting the docker-service. Then I followed the instructions found in
the [github repository](https://github.com/NVIDIA/nvidia-docker) to run the
image after it pulled it. Afterwards, I started a Jupyter Notebook that
required a token to login, which was specified in the output of the jupyter
command line prompt.

Once inside the jupyter notebook, I was finally able to run Tensroflow with
GPU acceleration.

Simple enough if you know where to look. But even if digging up took some
time, is time well spent considering the speedup achieved with GPU
acceleration when using Tensorflow.
