//inclusine di header che provvedono funzionalita
//qdebug serve a stampare a schermo
//qfle serve ad operare sui i file
//qstring serve ad effettuare delle operazioni sulle stringhe di testo
#include <QDebug>
#include <QFile>
#include <QString>
//config.cpp contiene la configurazione
#include "config.cpp"

//funzione che srotola i versetti in un capitolo
void decodeChapter(QString& capitolo) {
	int  inizioVersetto = 0;
	bool continua       = true;
	while (continua) {
		//cerco la posizione della stringa strong
		auto fineVersetto = capitolo.indexOf("<strong>", inizioVersetto + 1);
		//qDebug() << oldRos << newPos;
		if (fineVersetto == -1) {
			// se non e trovato l'ultimo lui capisce che deve andare alla fine del paragrafo
			fineVersetto = capitolo.indexOf("</p>", inizioVersetto + 1);
			continua     = false;
		}

		if (inizioVersetto > 0) {
			qDebug() << capitolo.mid(inizioVersetto, fineVersetto - inizioVersetto) << "\n\n ---------------------------- \n\n";
		}

		inizioVersetto = fineVersetto;
	}
}

int main() {
	//per scansionare i libri uno alla volta , ogni libro ha un suo id
	for (uint64_t offsetLibro = 0; offsetLibro < numeroLibri; ++offsetLibro) {
		//libro attuale e dato da dove iniziano i libri id5 a cui di volta in volta vado ad analizzare
		auto    libroAttuale = inizioLibri + offsetLibro;
		//stringa di testo
		QString percorso     = cartellaBase + QString::number(libroAttuale) + estensione;
		qDebug() << percorso;
		uint64_t capitoloAttuale = 1;
		for (;;) { //la condizione di arresto è complessa
			//ora scansioniamo i capitoli 2...N
			//i 2 piu vanno ad incrementare attuale+1
			capitoloAttuale++;
			QString percorsoCapitolo = cartellaBase + QString::number(libroAttuale) + "-split" + QString::number(capitoloAttuale) + estensione;
			qDebug() << percorsoCapitolo;
			QFile file(percorsoCapitolo);
			if (!file.open(QIODevice::ReadOnly)) {
				break; //termina il ciclo for, e avanza al prossimo libro
			} else {
				QString capitolo = file.readAll();
				decodeChapter(capitolo);
			}
		}
	}
}
