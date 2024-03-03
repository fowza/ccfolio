// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

#ifndef USER_ODB_HXX
#define USER_ODB_HXX

#include <odb/version.hxx>

#if (ODB_VERSION != 20400UL)
#error ODB runtime version mismatch
#endif

#include <odb/pre.hxx>

#include "User.h"

#include <cstddef>
#include <memory>
#include <utility>

#include <odb/callback.hxx>
#include <odb/container-traits.hxx>
#include <odb/core.hxx>
#include <odb/no-op-cache-traits.hxx>
#include <odb/pointer-traits.hxx>
#include <odb/result.hxx>
#include <odb/simple-object-result.hxx>
#include <odb/traits.hxx>
#include <odb/wrapper-traits.hxx>

#include <odb/details/shared-ptr.hxx>
#include <odb/details/unused.hxx>

namespace odb
{
// User
//
template <>
struct class_traits<::User>
{
    static const class_kind kind = class_object;
};

template <>
class access::object_traits<::User>
{
public:
    typedef ::User object_type;
    typedef ::User *pointer_type;
    typedef odb::pointer_traits<pointer_type> pointer_traits;

    static const bool polymorphic = false;

    typedef int id_type;

    static const bool auto_id = true;

    static const bool abstract = false;

    static id_type id(const object_type &);

    typedef no_op_pointer_cache_traits<pointer_type> pointer_cache_traits;

    typedef no_op_reference_cache_traits<object_type> reference_cache_traits;

    static void callback(database &, object_type &, callback_event);

    static void callback(database &, const object_type &, callback_event);
};
} // namespace odb

#include <odb/details/buffer.hxx>

#include <odb/sqlite/binding.hxx>
#include <odb/sqlite/forward.hxx>
#include <odb/sqlite/query.hxx>
#include <odb/sqlite/sqlite-types.hxx>
#include <odb/sqlite/version.hxx>

namespace odb
{
// User
//
template <typename A>
struct query_columns<::User, id_sqlite, A>
{
    // id
    //
    typedef sqlite::query_column<sqlite::value_traits<int, sqlite::id_integer>::query_type, sqlite::id_integer>
        id_type_;

    static const id_type_ id;

    // username
    //
    typedef sqlite::query_column<sqlite::value_traits<::std::string, sqlite::id_text>::query_type, sqlite::id_text>
        username_type_;

    static const username_type_ username;

    // passwordHash
    //
    typedef sqlite::query_column<sqlite::value_traits<::std::string, sqlite::id_text>::query_type, sqlite::id_text>
        passwordHash_type_;

    static const passwordHash_type_ passwordHash;

    // salt
    //
    typedef sqlite::query_column<sqlite::value_traits<::std::string, sqlite::id_text>::query_type, sqlite::id_text>
        salt_type_;

    static const salt_type_ salt;
};

template <typename A>
const typename query_columns<::User, id_sqlite, A>::id_type_ query_columns<::User, id_sqlite, A>::id(A::table_name,
                                                                                                     "\"id\"",
                                                                                                     0);

template <typename A>
const typename query_columns<::User, id_sqlite, A>::username_type_ query_columns<::User, id_sqlite, A>::username(
    A::table_name,
    "\"username\"",
    0);

template <typename A>
const typename query_columns<::User, id_sqlite, A>::passwordHash_type_ query_columns<::User, id_sqlite, A>::
    passwordHash(A::table_name, "\"passwordHash\"", 0);

template <typename A>
const typename query_columns<::User, id_sqlite, A>::salt_type_ query_columns<::User, id_sqlite, A>::salt(A::table_name,
                                                                                                         "\"salt\"",
                                                                                                         0);

template <typename A>
struct pointer_query_columns<::User, id_sqlite, A> : query_columns<::User, id_sqlite, A>
{
};

template <>
class access::object_traits_impl<::User, id_sqlite> : public access::object_traits<::User>
{
public:
    struct id_image_type
    {
        long long id_value;
        bool id_null;

        std::size_t version;
    };

    struct image_type
    {
        // id
        //
        long long id_value;
        bool id_null;

        // username
        //
        details::buffer username_value;
        std::size_t username_size;
        bool username_null;

        // passwordHash
        //
        details::buffer passwordHash_value;
        std::size_t passwordHash_size;
        bool passwordHash_null;

        // salt
        //
        details::buffer salt_value;
        std::size_t salt_size;
        bool salt_null;

        std::size_t version;
    };

    struct extra_statement_cache_type;

    using object_traits<object_type>::id;

    static id_type id(const id_image_type &);

    static id_type id(const image_type &);

    static bool grow(image_type &, bool *);

    static void bind(sqlite::bind *, image_type &, sqlite::statement_kind);

    static void bind(sqlite::bind *, id_image_type &);

    static bool init(image_type &, const object_type &, sqlite::statement_kind);

    static void init(object_type &, const image_type &, database *);

    static void init(id_image_type &, const id_type &);

    typedef sqlite::object_statements<object_type> statements_type;

    typedef sqlite::query_base query_base_type;

    static const std::size_t column_count = 4UL;
    static const std::size_t id_column_count = 1UL;
    static const std::size_t inverse_column_count = 0UL;
    static const std::size_t readonly_column_count = 0UL;
    static const std::size_t managed_optimistic_column_count = 0UL;

    static const std::size_t separate_load_column_count = 0UL;
    static const std::size_t separate_update_column_count = 0UL;

    static const bool versioned = false;

    static const char persist_statement[];
    static const char find_statement[];
    static const char update_statement[];
    static const char erase_statement[];
    static const char query_statement[];
    static const char erase_query_statement[];

    static const char table_name[];

    static void persist(database &, object_type &);

    static pointer_type find(database &, const id_type &);

    static bool find(database &, const id_type &, object_type &);

    static bool reload(database &, object_type &);

    static void update(database &, const object_type &);

    static void erase(database &, const id_type &);

    static void erase(database &, const object_type &);

    static result<object_type> query(database &, const query_base_type &);

    static unsigned long long erase_query(database &, const query_base_type &);

public:
    static bool find_(statements_type &, const id_type *);

    static void load_(statements_type &, object_type &, bool reload);
};

template <>
class access::object_traits_impl<::User, id_common> : public access::object_traits_impl<::User, id_sqlite>
{
};

// User
//
} // namespace odb

#include "User-odb.ixx"

#include <odb/post.hxx>

#endif // USER_ODB_HXX
