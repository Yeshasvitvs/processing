[mono, bi]=temporal_filters;
[even,odd]=createGabor;
size1=32;
double_size=size1*size1;

% 
% % Reshaping the Even and Odd kernels to vectors of 1Xsize1*size1
% Even=reshape(even,[1,size1*size1]);
% Odd=reshape(odd,[1,size1*size1]);
% 
% 
% for i=1:1:size1(bi,2)
% 
%     bi_odd(i,:)=Odd*bi(1,i);
%     mono_even(i,:)=Even*mono(1,i);
% 
% end
% 
% st=bi_odd + mono_even;
% 
% figure(1)
% subplot(2,2,1)
% surf(bi_odd);
% subplot(2,2,3)
% imagesc(bi_odd);
% 
% subplot(2,2,2)
% surf(mono_even);
% subplot(2,2,4)
% imagesc(mono_even)
% 
% figure(2)
% subplot(2,1,1)
% surf(st)
% subplot(2,1,2)
% imagesc(st)



%Reshaping the Even and Odd kernels to vectors of 1Xsize1*size1
Even=reshape(even,[1,double_size]);
Odd=reshape(odd,[1,double_size]);

BI_ODD=zeros(size1,size1);
MONO_EVEN=zeros(size1,size1);

for i=1:1:size(bi,2)

    bi_odd(i,:)=Odd*bi(1,i);
    mono_even(i,:)=Even*mono(1,i);

end


for i=1:1:size(bi_odd,1)

    temp_biodd=reshape(bi_odd(i,:),[size1,size1]);
    BI_ODD=BI_ODD+temp_biodd;
    
    temp_monoeven=reshape(mono_even(i,:),[size1,size1]);
    MONO_EVEN=MONO_EVEN+temp_monoeven;
    
    

end


ST=BI_ODD + MONO_EVEN;

figure(1)
subplot(2,2,1)
surf(BI_ODD);
subplot(2,2,3)
imagesc(BI_ODD);

subplot(2,2,2)
surf(MONO_EVEN);
subplot(2,2,4)
imagesc(MONO_EVEN)

figure(2)
subplot(2,1,1)
surf(ST)
subplot(2,1,2)
imagesc(ST)