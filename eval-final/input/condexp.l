main() 
entier $temp, entier $g, entier $l, entier $v;
{
  $temp = lire();                       # temperature de l'eau
  $g = 0;
  si ($temp < 0 | $temp = 0) alors {$g = 1;} # glace ? 
  $l = 0;
  si($temp < 100 & 0 < $temp) alors {$l = 1;} # liquide ?
  $v = 0;
  si(!$g & !$l) alors {$v = 1;} # vapeur ?
  ecrire($g);
  ecrire($l);  
  ecrire($v);  
}
