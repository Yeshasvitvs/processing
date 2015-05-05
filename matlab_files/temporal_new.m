% function [ft_mono, ft_bi]=temporal_new

    clear;
    clc;
% % % 	//Biphasic filter
% % 	 s1=0.5;
% %      s2=0.8;
% %      
% % 	 mu_bi1=0.25; %this is the parameter that has to be fixed 
% % 	 sigma_bi1= mu_bi1/3;
% % 
% % 	 
% % 	 mu_bi2 = 2 * mu_bi1;
% % 	 sigma_bi2 = mu_bi2/2;
% % 
% % % 	//Monophasic filter
% % 	
% % 	 mu_mono = (1/5) * (1 + mu_bi1 * sqrt(36 + log(s1/s2)) ) ;
% % 	 sigma_mono = mu_mono / 3;

% //Biphasic filter
	 s1=0.705;
     s2=0.8;
     
     step = 2;
     
	 mu_bi1= step * 0.18; %this is the parameter that has to be fixed 
	 sigma_bi1= step * 0.082;

	 
	 mu_bi2 = step * 0.42;
	 sigma_bi2 = step * 0.18;

% 	//Monophasic filter
	
	 mu_mono = step * 0.28;
	 sigma_mono = step * 0.07;


index = 1;

for time = 0:0.001:2;
    
    
    D_mono = (time - mu_mono)^2 / (2 * sigma_mono * sigma_mono);
    ft_mono(index)=exp(-D_mono);


    D1_bi= (time-mu_bi1)^2   /   (2 * sigma_bi1 * sigma_bi1);
    D2_bi= (time-mu_bi2)^2   /   (2 * sigma_bi2 * sigma_bi2);
    ft_bi(index)=(- s1 * exp(-D1_bi)) + (s2 * exp(-D2_bi) );
    
    index = index+1;

end

x=0:0.001:2;
figure(1);
% subplot(2,1,1)
plot(x,ft_mono,'.k');
% clf;
hold on;
plot(x,ft_bi,'.k');
% title('Monophasic');


% subplot(2,1,2)
% plot(x,ft_bi)
% title('Biphasic')
% end
