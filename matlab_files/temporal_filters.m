function [ft_mono, ft_bi]=temporal_filters

% 	//Monophasic filter
	 omega_m1_mono=1.95;
	 mu_m1_mono=0.55;
	 sigma_m1_mono=0.10;


	 omega_m2_mono=0.23;
	 mu_m2_mono=0.55;
	 sigma_m2_mono=0.16;

% 	//Biphasic filter
	 omega_m1_bi=1.83;
	 mu_m1_bi=0.44;
	 sigma_m1_bi=0.12;

	 omega_m2_bi=1.84;
	 mu_m2_bi=0.45;
	 sigma_m2_bi=0.12;



index=1;

for time=0:0.05:0.95
    
    
    D1_mono=-(	(time-mu_m1_mono)^2		/	(2*sigma_m1_mono*sigma_m1_mono)	);
    D2_mono=-(	(time-mu_m2_mono)^2		/	(2*sigma_m2_mono*sigma_m2_mono)	);
    ft_mono(index)=(omega_m1_mono*exp(D1_mono))-(omega_m2_mono*exp(D2_mono));


    D1_bi=-(    (time-mu_m1_bi)^2   /   (2*sigma_m1_bi*sigma_m1_bi) );
    D2_bi=-(    (time-mu_m2_bi)^2   /   (2*sigma_m2_bi*sigma_m2_bi) );
    ft_bi(index)=(omega_m1_bi*exp(D1_bi))-(omega_m2_bi*exp(D2_bi) );
    
    index=index+1;

end

x=0:0.05:0.95;
figure(1);
subplot(2,1,1)
plot(x,ft_mono)
title('Monophasic')


subplot(2,1,2)
plot(x,ft_bi)
title('Biphasic')
end
