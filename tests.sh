#!/bin/bash

function encrypt() {
  key="$1"
  plaintext="$2"
  pat="\([[:digit:]]+[[:blank:]]*,[[:blank:]]*[[:digit:]]+\)"
  res=$(echo -e "e\n$plaintext\n$key\nx" | $PROG | egrep -io "encrypted[^[:digit:]]*is:?[[:blank:]]*$pat" | egrep -o "$pat")
  if [ -z "$res" ]
  then
    echo "Not Found"
  else
    echo $res
  fi  
}

function decrypt() {
  key="$1"
  ciphertext="$2"
  pat="[[:digit:]]+"
  res=$(echo -e "d\n$ciphertext\n$key\nx" | $PROG | egrep -io "decrypted[^[:digit:]]*is:?[[:blank:]]*$pat" | egrep -o "$pat")
  if [ -z "$res" ]
  then
    echo "Not Found"
  else
    echo $res
  fi  
}

function pubkey() {
  privkey="$1"
  pat="[[:digit:]]+"
  res=$(echo -e "k\n$privkey\nx" | $PROG | egrep -io "public key[^[:digit:]]*is:?[[:blank:]]*$pat" | egrep -o "$pat")
  if [ -z "$res" ]
  then
    echo "Not Found"
  else
    echo $res
  fi  
}

#As most students hardcode p to 65537 use keys in this range 
function make_keys() {
  shuf -i 0-65537 -n10 > keys.txt
}

#As most students hardcode p to 65537 use numbers in this range 
function make_numbers() {
  shuf -i 0-65537 -n100 > numbers.txt
}

function test_round() {
  PRIVKEY=$1
  echo "# Starting round with private key $PRIVKEY"

  PUBKEY=$(pubkey "$PRIVKEY")
  if [ "$PUBKEY" == "Not Found" ]; then
    echo "Error. Failed to find public key output. Check Syntax."
    exit
  fi

  cat $NUMBERS | while read n; do
    secret="$(encrypt "$PUBKEY" "$n")"
    if [ "$secret" == "Not Found" ]; then
      echo "Error: Failed to find encryption output. Check Syntax."
      exit
    else
      nn=$(decrypt "$PRIVKEY" "$secret")
      if [ "$nn" == "Not Found" ]; then
        echo "Error: Failed to find decryption output. Check Syntax."
	exit
      else
        if test "$nn" != "$n"; then
          echo "Error: d(e($n)) = $nn, where e($n) = $secret, priv-key = $PRIVKEY, pub-key = $PUBKEY"
          exit
        fi
      fi
    fi    

  done

  echo "# Done round with private key $PRIVKEY"
}

function all_tests() {
  cat $KEYS | while read k; do
    test_round "$k"
  done
}

echo "# Testing small numbers"
PROG='./elGamal'
NUMBERS=numbers.txt
KEYS=keys.txt
all_tests
echo "# Done"
