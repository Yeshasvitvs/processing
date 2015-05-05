%Monophasic filter
omega_m1_mono=1.95;
mu_m1_mono=0.55;
sigma_m1_mono=0.10;

omega_m2_mono=0.23;
mu_m2_mono=0.55;
sigma_m2_mono=0.16;

%Biphasic filter
omega_m1_bi=0.83;
mu_m1_bi=0.44;
sigma_m1_bi=0.12;

omega_m2_bi=-0.34;
mu_m2_bi=0.63;
sigma_m2_bi=0.21;



omega_m1_mono.fh = figure('units','pixels',...
              'position',[300 300 300 100],...
              'menubar','none',...
              'name','Slider',...
              'numbertitle','off',...
              'resize','off');
omega_m1_mono.sl = uicontrol('style','slide',...
                 'unit','pix',...
                 'position',[20 10 260 30],...
                 'min',-1,'max',1,'val',0,'Callback', @omega_m1_mono_sliderCallback);             

             
