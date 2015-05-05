clear;
clc; 
data1 = []; 
data2 = []; 
data = []; 
temp = zeros(401,401); 
im = [];
real = [];
u = 0; v = 0; p=0;
i = 0;
for t = 0:0.01:1; 
    p= p +1 ;
    for x = -10:0.05:10; 
        u = u + 1;
       for y= -10:0.05:10;
            v = v + 1;
          
          [ data1(v,u) data2(v,u) data(v,u)] = computeSTG(x, y, t);
            
                 
        end
        v = 0;
    end
    u=0;
%     temp = temp + data;
    figure(5); clf; 
    title('Oriented Spatio-Temporal Filter');
    axis([0 400 0 400] );
    axis square;
    cla(gca);
    hold on;
    mesh(data);
    hold off;
    colorbar;
%    caxis([-0.1 0.1])
    view(180,0);
%     view(-48,10);
% %     figure(6); clf; 
% %     title('EVEN SPATIAL - BIPHASIC ');
% %     axis([0 400 0 400] );
% %     axis square;
% %     cla(gca);
% %     hold on;
% %     mesh(data1);
% %     hold off;
% %     view(-140,3);
% %     colorbar;
% %     figure(7); clf; 
% %     title('ODD SPATIAL - MONOPHASIC');
% %     axis([0 400 0 400] );
% %     axis square;
% %     cla(gca);
% %     hold on;
% %     mesh(data2);
% %     hold off;
% %     view(-140,3);
% %     
% %     colorbar;
    pause(1/60);
end


% % for x = -10:0.05:10;
% %     u = u + 1;
% %     for y = -10:0.05:10;
% %         p= p +1 ;
% % %         t=0;
% %        for t = -0.2:0.01:1;
% %             v = v + 1;
% %           
% %             [ data1(v,u) data2(v,u) data(v,u)] = computeSTG(x, y, t);
% % %                 act = computeSTG(x, y, t);
% % %               [im(u,p) real(u,p)] = computeSTG(x, y, t);
% % %         if data(v,u) > 0.2e-4
% % %                 
% % %                 i = i + 1;
% % %                 value(i, :) = [x y t];
% % %         end
% % %             
% %             
% %         end
% %         v = 0;
% %     end
% %     p=0;
% % end
%
%plots of Gabors 
% figure(1)
% subplot(1,2,1)
% surf(real);
% subplot(1,2,2)
% surf(im);


%plots of oriented spatial filters 
% figure(2); clf; hold on;
% % subplot(1,3,1)
% imagesc(data1);
% colorbar;
% title('EVEN-BI')
% figure(3); clf; hold on;
% % subplot(1,3,2)
% imagesc(data2);
% colorbar;
% title('ODD-MONO')
% % subplot(1,3,3)
% figure(4); clf; hold on;
% imagesc(data);
% title('ORIENTED SPATIO-TEMPORAL')
% colorbar;

% figure(6)
% display('Now Plotting');
% plot3(value(:, 1), value(:, 2), value(:, 3),'.')
% xlabel('x');
% ylabel('y');
% zlabel('t');

return 
