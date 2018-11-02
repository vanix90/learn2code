#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QString>

const QString  cartellaBase = "/home/cham/Downloads/jw/nwt_E/OEBPS/";
const QString  estensione   = ".xhtml";
const uint64_t inizioLibri  = 1001061105;
const uint64_t numeroLibri  = 66;

//funzione che srotola i versetti in un capitolo
void decodeChapter(QString capitolo) {
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
	for (uint64_t offsetLibro = 0; offsetLibro < numeroLibri; ++offsetLibro) {
		auto    libroAttuale = inizioLibri + offsetLibro;
		QString percorso     = cartellaBase + QString::number(libroAttuale) + estensione;
		qDebug() << percorso;
		uint64_t capitoloAttuale = 1;
		for (;;) { //la condizione di arresto è complessa
			//ora scansioniamo i capitoli 2...N
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
			exit(1);
		}
	}
}
