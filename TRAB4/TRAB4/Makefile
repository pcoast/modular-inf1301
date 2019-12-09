# Since the blah target is first, it is the default target and will be run when we run "make"

gch:
	rm *.gch

compile:
	gcc -Wall -o 'ArcaboucoTeste.lib' 'EXEC/lista' 'CESPDIN.C' 'CONTA.C' 'GENERICO.C' 'GERAALT.C' 'INTRPCED.C' 'INTRPCNT.C' 'LERPARM.C' 'LISTA.C' 'PRINCIP.C' 'TABSIMB.C' 'TESTLIS.C'

compile-no-warning:
	gcc -w -o 'EXEC/lista' 'ArcaboucoTeste.lib' 'CESPDIN.C' 'CONTA.C' 'GENERICO.C' 'GERAALT.C' 'INTRPCED.C' 'INTRPCNT.C' 'LERPARM.C' 'LISTA.C' 'PRINCIP.C' 'TABSIMB.C' 'TESTLIS.C'

run:
	./EXEC/lista /sEXEC/TesteLista.script