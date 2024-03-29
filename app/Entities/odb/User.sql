/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS "User" CASCADE;

CREATE TABLE "User" (
  "id" SERIAL NOT NULL PRIMARY KEY,
  "username" TEXT NOT NULL,
  "passwordHash" TEXT NOT NULL,
  "salt" TEXT NOT NULL);

CREATE INDEX "User_salt_i"
  ON "User" ("salt");
