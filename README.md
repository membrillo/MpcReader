# MpcReader
MpcReader

CHANGELOG
Primera versión
Estas lineas no hacen nada:
aux=mpc.bus(mpc.bus(:,3)>0,1);
gen_v=zeros(size(aux,1),size(mpc.gen,2));
gen_v(:,1)=aux;
gen_v(:,8)=2;
gen_v(:,9)=999;
gen_v(:,14)=1;
cost_v(:,6)=20*500;
