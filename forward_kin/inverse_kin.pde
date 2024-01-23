
float x=125,y=125;
float l1=125;
float l2=125;
float totalangle;
float ang1=90;;
float ang2=100;
totalangle=acos((x/sqrt(sq(x) + sq(y))));//(y/x))
ang2=((sq(l1)+sq(l2)-sq(x)-sq(y)))/(2*l1*l2);
ang2=180-(acos(ang2)*(180/(PI)));
ang1=(sq(x)+sq(y)+sq(l1)-sq(l2))/(2*sqrt(sq(x) + sq(y))*l1);
ang1=(acos(ang1)*(180/(PI)))+totalangle*(180/(PI));

print(ang1);
print("   ");
print(ang2);
