
sigma=0.5;
zeta=100;
theta=0:0.25*pi:pi;
% theta=0;
center(1)=16;
center(2)=16;

W=1/(2*pi*sigma*sigma);



for i=1:32
for j=1:32
   
    pixel = [i,j];
  pixel_tr=[i*cos(theta)+j*sin(theta),-i*sin(theta)+j*cos(theta)];

%     pixel_tr=[(i-center(1))*sin(theta)-(j-center(2))*cos(theta),i*cos(theta)+j*sin(theta)];
    
%     D=( (pixel_tr(1))^2 + (pixel_tr(2))^2 )/(sigma*sigma);
    D=( (pixel_tr(1)-center(1))^2 + (pixel_tr(2)-center(2))^2 )/(sigma*sigma);
    E=exp(-pi*D);
    C1= cos(zeta*(i+j));
    C2= sin(zeta*(i+j));
%     C=exp(0:+ (zeta*(i+j)));
    G1(i,j)=W*E*C1;
    G2(i,j)=W*E*C2;
    
    
    
end
end

figure(1)
subplot(2,2,1)
title('Gabor')
surf(G1)
subplot(2,2,3)
imagesc(G1)

figure(1)
subplot(2,2,2)
title('Gabor')
surf(G2)
subplot(2,2,4)
imagesc(G2)

