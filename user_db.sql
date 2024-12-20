-- mysql -uusermanager -pcyh153600!@ < user_db.sql

use users;
drop table if exists sign_in_log;
drop table if exists movement_log;
drop table if exists chat_log;
drop table if exists IDandPw;

create table IDandPw(
    ID varchar(16) not null,
    PW varchar(64) not null,

    primary key (ID)
);

desc IDandPw;

create table sign_in_log(
    slog_id int not null AUTO_INCREMENT,
    userID varchar(16) not null,

    port int not null,
    client_ip VARCHAR(45) not null,

    connected_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP not null,

    primary key (slog_id),
    foreign key (userID) references IDandPw(ID)
);
desc sign_in_log;

create table chat_log(
    clog_id int not null AUTO_INCREMENT,
    userID varchar(16) not null,
    chat varchar(512),

    primary key (clog_id),
    foreign key (userID) references IDandPw(ID)
);
desc chat_log;

create table movement_log(
    mlog_id int not null AUTO_INCREMENT,
    userID varchar(16) not null,
    pos_x int,
    pos_y int,

    primary key (mlog_id),
    foreign key (userID) references IDandPw(ID)
);
desc movement_log;

commit;