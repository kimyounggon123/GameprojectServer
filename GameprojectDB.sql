-- use this log-in line
-- mysql -uroot -p153600 < GameprojectDB.sql

DROP DATABASE IF EXISTS users;
DROP USER IF EXISTS usermanager@localhost;

use mysql;

-- make database and db local user.
create user usermanager@localhost identified by 'cyh153600!@';
create database users;
grant all privileges on users.* to usermanager@localhost with grant option;

commit;

