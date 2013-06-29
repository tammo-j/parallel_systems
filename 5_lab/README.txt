Parallel Systems Assignment 5 - Lösung
von Tammo Johannes Herbert (319391), Rico Jasper (319396), Erik Rudisch (343930)

Lösung setzt für 1a) und 1b) wurden in C implementiert. Es existieren drei Versionen der FIFO-Queue:
	- fifo_0: Vorlage
		* Die Implementierung entspricht der Vorlage.
		* Allerdings wurde die Queue-Datenstruktur von der main-Funktion getrennt und in eine separate Datei ausgegliedert.
	- fifo_a: feinkörnige Synchronisation
		* Hier verwenden wir zwei Mutexe für Head und Tail.
		* Außerdem haben wir zwei Dummies für eingeführt. Notwendig wäre allerdings nur ein Head-Dummy gewesen.
	- fifo_b: Lock-freie Synchronisation
		* Auf Mutexe wurde hier verzichtet.
		* Konsistenz wird durch atomare CAS-Instruktionen gewährleistet.
		* Wir benutzen hier nur ein Dummy für den Kopf der Queue.
		* Unvollständigen Dequeue-Operationen wird durch Weitersetzen des Tail-Pointers geholfen.
Wir benutzen für alle FIFO implementierungen dieselbe main.c Quelldatei. Im Makefile wird zur Zeit nur fifo_b eingebunden. Um die anderen Varianten zu kompilieren muss die FIFO-Implementierung entsprechend ausgetauscht werden.
