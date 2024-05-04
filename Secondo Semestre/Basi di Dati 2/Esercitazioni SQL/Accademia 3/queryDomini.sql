-- SCHEMA: prova

-- DROP SCHEMA IF EXISTS prova ;

CREATE SCHEMA IF NOT EXISTS prova
    AUTHORIZATION postgres;

create type prova.Strutturato as enum ('Ricercatore', 'Professore Associato', 'Professore Ordinario');

create type prova.LavoroProgetto as enum ('Ricerca e Sviluppo', 'Dimostrazione', 'Management', 'Altro');

create type  prova.LavoroNonProgettuale as enum ('Didattica', 'Ricerca', 'Missione', 'Incontro Dipartimentale', 'Incontro
Accademico', 'Altro');
create type prova.CausaAssenza as enum ('Chiusura Universitaria', 'Maternita', 'Malattia');

create domain prova.PosInteger as integer
default 0
check (value>=0);

CREATE domain prova.StringaM AS VARCHAR(100);

create domain prova.NumeroOre as integer
default 0
check (value>=0 and value<=8);

create domain prova.Denaro as real
check (value >=0);