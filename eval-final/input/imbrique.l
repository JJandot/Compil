entier $i, entier $j, entier $k, entier $somme;
main() {
  $somme = 0;
  $i = 0;
  tantque $i < 10 faire{
    $j = 0;
    tantque $j < $i faire {
      $k = 0;
      tantque  $k < $j faire{
        $somme = $somme + $i + $j + $k;
        $k = $k + 1;
        }
      $j = $j + 1;
      }
    $i = $i + 1;
    }
  ecrire ( $somme );
}
