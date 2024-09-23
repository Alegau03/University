create table prova.Persona(
	id prova.posinteger not null,
	nome prova.stringam,
	cognome prova.stringam,
	posizione prova.strutturato,
	stipendio prova.denaro,
	primary key(id)
); 


create table prova.Progetto(

	id prova.posinteger not null,
	nome prova.stringam not null,
	inizio date,
	fine date check (inizio < fine),
	budget prova.denaro,

	primary key(id),
	unique(nome)
);

create table prova.WP(

	progetto prova.posinteger not null,
	id prova.posinteger not null,
	nome prova.stringam not null,
	inizio date,
	fine date check (inizio < fine),

	primary key(progetto, id),
	unique (progetto, nome),
	foreign key (progetto) references prova.Progetto(id)
);


create table prova.AttivitaProgetto(

	id prova.posinteger not null,
	persona prova.posinteger not null,
	progetto prova.posinteger not null,
	wp prova.posinteger not null,
	giorno date,
	tipo prova.lavoroprogetto,
	oreDurata prova.numeroore,

	primary key(id),
	foreign key(persona) references prova.Persona(id),
	foreign key(progetto,wp) references prova.WP(progetto,id)
);


create table prova.AttivitaNonProgettuale(

	id prova.posinteger not null,
	persona prova.posinteger not null,
	tipo prova.lavorononprogettuale,
	giorno date,
	oreDurata prova.numeroore,

	primary key(id),
	foreign key(persona) references prova.Persona(id)
);


create table prova.Assenza(

	id prova.posinteger not null,
	persona prova.posinteger not null,
	tipo prova.causaassenza,
	giorno date,

	primary key(id),
	unique (persona,giorno),
	foreign key(persona) references prova.Persona(id)
);
