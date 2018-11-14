all : hash trans
hash : hashfile.c
	gcc -Wall -o hashfile hashfile.c product.c price.c tokenizer.c hashbuilder.c
trans : updateinventory.c
	gcc -Wall -o update hashfile.c product.c price.c tokenizer.c updateinventory.c

