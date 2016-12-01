知乎上有几个比较好的讲解傅里叶变换的文章：
[傅里叶分析之掐死教程（完整版）](https://zhuanlan.zhihu.com/p/19763358)
通过这些文章都能对频谱有大致了解，但等你自己坐下了，要对一个信号进行频谱分析时，你会发现好多细微的问题其实并没有注意，下面，将讲讲那些细微的问题

#实现快速傅里叶变换

忠告：除非你自己为了验证你的能力，或为了验证你对对快速傅里叶变换算法的了解，千万别用自己写的快速傅里叶变换算法，也别在网上随便找一个算法就拿来用，快速傅里叶变换算法全世界比较权威的有[FFTW](http://www.fftw.org/)，是由其由MIT(麻省理工学院)的M.Frigo 和S. Johnson 开发，可计算一维或多维实和复数据以及任意规模的DFT。当然，你如果不屑于人家一个学院开发的快速傅里叶变换库，你用你自己写的快速傅里叶变换算法没人拦着你，不过你如果不是用汇编来写那你已经输在了起跑线上了。
废话说那么多就是让你别用自己写的快速傅里叶变换算法，推荐大家使用人家经过十几年磨练的成熟库，其中FFTW就是一个很好的选择

#FFT

工程中遇到的问题都是这样的：有一个信号，采样率为fs，求频谱

首先用一段matlab的代码进行成信号
```matlab
N = 256;
t = linspace(0,2*pi,256);
Fs = 100;
t = [0:N-1]./Fs;

wave = 1*cos(2*pi*10.*t) ...
    + 2*sin(2*pi*15.*t + deg2rad(30)) ...
    + 3*cos(2*pi*20.*t + deg2rad(-30)) ...
    + 4*sin(2*pi*26.5.*t + deg2rad(60)) ...
    ;
Fs = 1/(t(2)-t(1));                    
```
wave为生成的信号，Fs为采样率，这段信号由形如Acos(2πωt+θ)的基本信号组成，中学物理知识都知道，Acos(2πωt+θ)中，A代表幅值，ω为角频率，θ为相位角，傅里叶变换就是把一段信号分解为n个形如Acos(2πωt+θ)基本信号叠加的过程
上面那段信号进行绘制：
```matlab
figure
plot(t,wave);
set(gcf,'color','w');
xlabel('time (s)');
ylabel('Amp');
title('信号');
```

![这里写图片描述](http://img.blog.csdn.net/20160620154334852)
这段数据如下：
> 8.06217782649107	6.69261843058528	-3.35043720308466	-4.66253989242961	0.697246349738525	0.0753692287562088	-2.78513076487351	0.499925017089664	3.17606212123038	-0.277082029119007	-1.05610561921709	3.01302269453115	1.03514268896465	-5.45859004974005	-2.40431451359648	6.43871086475007	3.84304060556592	-6.54073038281710	-6.18090623900590	3.18876994983517	5.42972297462435	1.05400419914209	0.343298598331839	0.280851846782554	
……
-3.33193590622286	4.25926639098748	4.59807621135344	-2.58753675611741	-3.56492073452458	1.00777782079415	0.978670962706604	-0.664391120096073

下面我们用matlab的fft函数对这个数据进行快速傅里叶变换：

>7.38504326171959+0.00000000000000i	**7.38708694461654+0.183505522698889i**	7.39321771956158+0.368190251593997i	7.40343404206541+0.555266982451810i	7.41773083839269+0.746017837381402i	7.43609543142577+0.941834538704739i	7.45850113316254+1.14426613813919i	7.48489766109681+1.35507803033555i	7.51519706012674+1.57632754978198i	7.54925308382079+1.81046378661169i	
……
7.58683083724872-2.06046299281095i	7.54925308382079-1.81046378661169i	7.51519706012674-1.57632754978198i	7.48489766109681-1.35507803033555i	7.45850113316254-1.14426613813919i	7.43609543142577-0.941834538704739i	7.41773083839269-0.746017837381402i	7.40343404206541-0.555266982451810i	7.39321771956158-0.368190251593997i	**7.38708694461654-0.183505522698889i**


傅里叶变换出来的是对应长度的一堆复数，
此复数数组符合如下规律:

 - 第一个（索引为0)和N/2的两个复数的虚数部分为0;
 - 下标为i和N-i的两个复数共轭，也就是其虚数部分数值相同、符号相反.[1]
 
 把它的模绘图可以看到其特性
 
```matlab
 Y = fft(wave);
figure
plot(abs(Y));
set(gcf,'color','w');
```

![直接fft](http://img.blog.csdn.net/20160620173844523)

由于虚数部共轭和虚数部为0等规律，真正有用的信息保存在下标从0到N/2的N/2+1个虚数中
下面让我们来看看FFT变换之后的那些复数都代表什么意思。


#FFT之后得到的是什么数

FFT之后得到的那一串复数是波形对应频率下的幅度特征，注意这个是幅度特征不是复制，下面要讲两个问题：1.如何获取频率，2.如何获取幅值

##获取频率

FFT变换如何获取频率？傅里叶变换并没对频率进行任何计算，频率只与采样率和进行傅里叶变换的点数相关，注意这里是进行傅里叶变换的点数而不一定是信号的长度。
FFT变换完第一个数时0Hz频率，0Hz就是没有波动，没有波动有个专业一点的说法，叫**直流分量**。
后面第二个复数对应的频率是0Hz+频谱分辨率，每隔一个加一次，频谱分辨率$\Delta f $计算公式如下：
$$ 
\Delta f = \frac{{Fs}}{N} 
$$
式中：
Fs为采样率
N为FFT的点数
因此只要Fs和N定了，频域就定下来了。


##FFT变换后的第一个实数 - 直流分量

FFT之后的第一个结果表示了时域信号中的直流成分的多少，所谓直流信号，代表和基准0的偏移量。
上面的结果不好说明，下面再看一个例子：
```matlab
oneWave = [1,1,1,1,1,1,1,1];
fft(oneWave)
```
输出：

> 8	0	0	0	0	0	0	0

oneWave 的直流分量是1，但计算结果是8，这里又引入一个问题，FFT之后的数值不是真实的幅值，需要进行转转换，第一个点需要除以N，才能还原为原来的结果

##FFT变换后的复数模 - 幅度

假设原始信号的峰值为A，那么FFT的结果的每个点（除了第一个点直流分量之外）的模值就是A
的N/2倍。而第一个点就是直流分量，它的模值就是直流分量的N倍

也就是说，要得出真实幅值，需要把除了第1个点(i=0)以及最后一个点(i=N/2)除以N以外，其余点需要求得的模除以N/2
这是因为傅里叶级数对应时域幅值，其中已经包含了1/N项，而fourier变换中没有该系数，
所以，进行完fft变换后需除以N/2才能与时域对应上。
FFT的计算公式
$$
Fn = \sum\limits_{i = 0}^{N - 1} {{x_i}{e^{ - \frac{{2\pi j}}{N}ni}}} 
$$
实际应用中，只有i=0~N/2是有用的
全世界绝大部分的FFT算法计算出来后都需要进行幅度的转换的，幅值根据需求有不同需求，具体见下节

#幅度谱，幅值谱？Magnitude，Amplitude？

- 幅值 Amplitude
幅值就是对于波形的幅值来说的，上面一节说的转换就是把fft计算的结果转化为幅值，英文叫Amplitude
在工程中还经常看到分贝纵坐标的频谱，带分贝的频谱，使用分贝数的好处是，用较小的坐标可以描述很宽的范围。工程上会取20log(Amplitude)转变为分贝。
幅值第n(其中n!=1)点处的fft计算的结果是复数a+bi，模值A=sqrt(a^2+b^2)，那么实际信号的幅值是2*A/N;
当n=0时(0Hz)，也就是第一个点就是直流分量，它的模值就是直流分量的N倍，实际信号的幅值是A/N，**注意N是采样点而不是进行FFT的点数**


- 幅度 Magnitude
若对fft的结果不做任何处理，直接取模，那么这个值叫幅度，英文上叫Magnitude，


于是对fft计算的复数结果，其实数和虚数对应如下：

| 名称 | 计算公式 |
|--|:-------:|
| 幅度(Magnitude) | $\sqrt {{{{\mathop{\rm Re}\nolimits} }^2} + {{{\mathop{\rm Im}\nolimits} }^2}} $ |
| 幅值(Amplitude) |$\left\{ {\begin{array}{*{20}{c}}{\frac{{\sqrt {{{{\mathop{\rm Re}\nolimits} }^2} + {{{\mathop{\rm Im}\nolimits} }^2}} }}{n},i = (0,\frac{n}{2})}\\{\frac{{2\sqrt {{{{\mathop{\rm Re}\nolimits} }^2} + {{{\mathop{\rm Im}\nolimits} }^2}} }}{n},otherwise}\end{array}} \right.$|
|dB|20log(Amplitude)|
根据这个表，就可以很明白FFT之后需要进行什么样的处理了。

#信号补零-提高频谱显示分辨率
有了上面的知识，做出频谱是没有太大问题了，但在频谱分析前，还需要说另外一个问题，前文提到，获取频率频谱需要计算频谱分辨率$ \Delta f = \frac{{Fs}}{N} $。
频谱分辨率数值越小，频谱就越精细，分辨率越高，所以，在一个时间里，能采集的点越多越好。
特别是在采样率高的情况下，采样率作为分子，是降低分辨率的一个因素，因此高频采样中，能采集的点越多越好。

另外吐槽一下，采集前别乱选采样率，要对采集的信号有一定的了解，确定大致感兴趣的频率段，频谱分析的频率范围是[0~Fs/2]，也就是采样率的一半是你频谱的极限。别瞎选，以前就看过不懂采集时选了一个最大的采样率，结果得出来的频谱质量非常差。

在采集点数不足时，有一个方法可以提高频谱分辨率，就是信号补零。**注意**，这个提高只是视觉上的提高，并没有再物理上有相应的提高。也就是没有的频率成分你补零之后还是没有。

一般如果信号不是2^n的长度，会补零把信号补到2^n的长度，这样是因为2^n长度的傅里叶信号计算会更快更准。

#完整的频谱分析代码

简单起见，这里贴上matlab的代码和Python的代码：
首先matlab：
定义一个函数进行频谱分析
只要输入data波形，Fs采样率，就可以输出Fre和Amp
File:
frequencySpectrum.m
```matlab
function [Fre,Amp,Ph,Fe] = frequencySpectrum( wave,Fs,varargin)
    %傅里叶变换
    %   data:波形数据
    %   Fs:采样率
    %   varargin:
    %		isaddzero->是否补零，默认为1，否则会按照data的长度进行fft
    %		scale->幅值的尺度，'amp'为幅值谱，'ampDB'为分贝显示的幅值谱,'mag'为幅度谱就是fft之后直接取模,'magDB'为'mag'对应的分贝
    %		isdetrend->是否进行去均值处理，默认为1
    %   得到的是[fre:频率,Amp:幅值,Ph:相位,Fe:原始的复数]

    if (size(wave,1)>1 && size(wave,2) > 1)
    	for i=1:size(wave,2)
    		[a,b,c,d] = frequencySpectrum_1dim( wave(:,i),Fs,varargin);
    		Fre(:,i) = a;
    		Amp(:,i) = b;
    		Ph(:,i) = c;
    		Fe(:,i) = d;
    	end
    else
    	[Fre,Amp,Ph,Fe] = frequencySpectrum_1dim( wave,Fs,varargin);
    end
end

function [Fre,Amp,Ph,Fe] = frequencySpectrum_1dim( data,Fs,varargin)
    %傅里叶变换
    %   data:波形数据
    %   Fs:采样率
    %   varargin:
    %		isaddzero->是否补零，默认为1，否则会按照data的长度进行fft
    %		scale->幅值的尺度，'amp'为幅值谱，'ampDB'为分贝显示的幅值谱,'mag'为幅度谱就是fft之后直接取模,'magDB'为'mag'对应的分贝
    %		isdetrend->是否进行去均值处理，默认为1
    %   得到的是[fre:频率,Amp:幅值,Ph:相位,Fe:原始的复数]
    isAddZero = 1;
    scale = 'amp';
    isDetrend = 1;
    while length(varargin)>=2
        prop =varargin{1};
        val=varargin{2};
        varargin=varargin(3:end);
        switch lower(prop)
            case 'isaddzero' %是否允许补0
                isAddZero = val;
            case 'scale'
                scale = val;
            case 'isdetrend'
                isDetrend = val;
        end
    end

    n=length(data);
    if isAddZero
        N=2^nextpow2(n);
    else
        N = n;
    end
    
    if isDetrend
        Y = fft(detrend(data,'constant'),N);
    else
        Y = fft(data,N);
    end
    
    Fre=(0:N-1)*Fs/N;%频率
    Fre = Fre(1:N/2);
    Amp = dealMag(Y,N,n,scale);
    ang=angle(Y(1:N/2));
    Ph=ang*180/pi;
    Fre = Fre';
    Fe = Amp.*exp(1i.*ang);
end

function amp = dealMag(fftData,fftSize,dataSize,scale)
	switch lower(scale)
		case 'amp'
			amp=abs(fftData);
			amp(1)=amp(1)/dataSize;
			amp(2:fftSize/2-1)=amp(2:fftSize/2-1)/(dataSize/2);
			amp(fftSize/2)=amp(fftSize/2)/dataSize;
			amp=amp(1:fftSize/2);
		case 'ampdb'
			amp=abs(fftData);
			amp(1)=amp(1)/fftSize;
			amp(2:fftSize/2-1)=amp(2:fftSize/2-1)/(fftSize/2);
			amp(fftSize/2)=amp(fftSize/2)/fftSize;
			amp=amp(1:fftSize/2);
			amp = 20*log(amp);
		case 'mag'
			amp=abs(fftData(1:fftSize/2));
		case 'magdb'
			amp=abs(fftData(1:fftSize/2));
			amp = 20*log(amp);
		otherwise
			error('unknow scale type');
	end
end
```

调用示例
```matlab
N = 256;
t = linspace(0,2*pi,256);
Fs = 100;
t = [0:N-1]./Fs;

waveData = 1*cos(2*pi*10.*t) ...
    + 2*sin(2*pi*15.*t + deg2rad(30)) ...
    + 3*cos(2*pi*20.*t + deg2rad(-30)) ...
    + 4*sin(2*pi*26.5.*t + deg2rad(60)) ...
    ;
Fs = 1/(t(2)-t(1));          

figure
[Fre,Amp] = frequencySpectrum(waveData,Fs);
subplot(2,2,1)
plot(Fre,Amp);
set(gcf,'color','w');
title('amp')

subplot(2,2,2)
[Fre,Amp] = frequencySpectrum(waveData,Fs,'scale','ampdb');
plot(Fre,Amp);
set(gcf,'color','w');
title('ampDB')

subplot(2,2,3)
[Fre,Amp] = frequencySpectrum(waveData,Fs,'scale','mag');
plot(Fre,Amp);
set(gcf,'color','w');
title('mag')

subplot(2,2,4)
[Fre,Amp] = frequencySpectrum(waveData,Fs,'scale','magdb');
plot(Fre,Amp);
set(gcf,'color','w');
title('magDB')

```

![这里写图片描述](http://img.blog.csdn.net/20160627173854536)


python版本的见GitHub：
https://github.com/czyt1988/DataProcess/blob/master/czy/signal.py
函数spectrum

C++版本的等我写下一篇文章吧：

#总结

- FFT之后的幅值是需要进行特殊处理才能使用的，并非直接对应我们物理上的幅值，需要进行换算，换算的方法见上文列表

-  FFT的数据点数可以是原有数据的点数，也可以认为补长，使其有一定的可视分辨率



其实还有：

- 截断加窗问题

- 加窗频谱幅值修正问题

等闲的时候再写写了

若有错误请大家指正

#参考文献
[1][用Python做科学计算](http://old.sebug.net/paper/books/scipydoc/fft_study.html)