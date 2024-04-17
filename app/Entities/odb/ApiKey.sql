/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS "ApiKey" CASCADE;

CREATE TABLE "ApiKey" (
  "id" BIGSERIAL NOT NULL PRIMARY KEY,
  "hash" TEXT NOT NULL,
  "description" TEXT NOT NULL,
  "status" TEXT NOT NULL,
  "user_id" BIGINT NOT NULL,
  "expires_at" TIMESTAMP NULL,
  "created_at" TIMESTAMP NOT NULL DEFAULT 'CURRENT_TIMESTAMP',
  "updated_at" TIMESTAMP NOT NULL DEFAULT 'CURRENT_TIMESTAMP');
