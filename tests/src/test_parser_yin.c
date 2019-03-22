#include "../../src/common.c"
#include "../../src/set.c"
#include "../../src/log.c"
#include "../../src/hash_table.c"
#include "../../src/xpath.c"
#include "../../src/parser_yang.c"
#include "../../src/context.c"
#include "../../src/tree_schema_helpers.c"
#include "../../src/tree_schema_free.c"
#include "../../src/tree_schema_compile.c"
#include "../../src/tree_schema.c"
#include "../../src/xml.c"
#include "../../src/parser_yin.c"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>

#include "libyang.h"

struct state {
    struct ly_ctx *ctx;
    struct lys_module *mod;
};

static int
setup_f(void **state)
{
    struct state *st = NULL;

    /* allocate state variable */
    (*state) = st = calloc(1, sizeof(*st));
    if (!st) {
        fprintf(stderr, "Memmory allocation failed");
        return EXIT_FAILURE;
    }

    /* create new libyang context */
    ly_ctx_new(NULL, 0, &st->ctx);

    /* allocate new module */
    st->mod = calloc(1, sizeof(*st->mod));
    st->mod->ctx = st->ctx;

    return EXIT_SUCCESS;
}

static int
teardown_f(void **state)
{
    struct state *st = *(struct state **)state;

    lys_module_free(st->mod, NULL);
    ly_ctx_destroy(st->ctx, NULL);
    free(st);

    return EXIT_SUCCESS;
}

static void
test_parse(void **state)
{
    LY_ERR ret = LY_SUCCESS;
    struct state *st = *state;

    ret = yin_parse_module(st->ctx,
                    "<module name=\"example-foo\"\
                        xmlns=\"urn:ietf:params:xml:ns:yang:yin:1\"\
                        xmlns:foo=\"urn:example:foo\"\
                        xmlns:myext=\"urn:example:extensions\">\
                        <namespace uri=\"urn:example:foo\"/>\
                        <prefix value=\"foo\"/>\
                    </module>",
                st->mod);

    assert_int_equal(ret, LY_SUCCESS);
    assert_string_equal(st->mod->parsed->mod->name, "example-foo");
    assert_string_equal(st->mod->parsed->mod->prefix, "foo");
}

static void
test_match_keyword(void **state)
{
    (void)state; /* unused */

    assert_int_equal(match_keyword("anydatax", strlen("anydatax"), 0), YANG_NONE);
    assert_int_equal(match_keyword("asdasd", strlen("asdasd"), 0), YANG_NONE);
    assert_int_equal(match_keyword("", 0, 0), YANG_NONE);
    assert_int_equal(match_keyword("anydata", strlen("anydata"), 0), YANG_ANYDATA);
    assert_int_equal(match_keyword("anyxml", strlen("anyxml"), 0), YANG_ANYXML);
    assert_int_equal(match_keyword("argument", strlen("argument"), 0), YANG_ARGUMENT);
    assert_int_equal(match_keyword("augment", strlen("augment"), 0), YANG_AUGMENT);
    assert_int_equal(match_keyword("base", strlen("base"), 0), YANG_BASE);
    assert_int_equal(match_keyword("belongs-to", strlen("belongs-to"), 0), YANG_BELONGS_TO);
    assert_int_equal(match_keyword("bit", strlen("bit"), 0), YANG_BIT);
    assert_int_equal(match_keyword("case", strlen("case"), 0), YANG_CASE);
    assert_int_equal(match_keyword("choice", strlen("choice"), 0), YANG_CHOICE);
    assert_int_equal(match_keyword("config", strlen("config"), 0), YANG_CONFIG);
    assert_int_equal(match_keyword("contact", strlen("contact"), 0), YANG_CONTACT);
    assert_int_equal(match_keyword("container", strlen("container"), 0), YANG_CONTAINER);
    assert_int_equal(match_keyword("default", strlen("default"), 0), YANG_DEFAULT);
    assert_int_equal(match_keyword("description", strlen("description"), 0), YANG_DESCRIPTION);
    assert_int_equal(match_keyword("deviate", strlen("deviate"), 0), YANG_DEVIATE);
    assert_int_equal(match_keyword("deviation", strlen("deviation"), 0), YANG_DEVIATION);
    assert_int_equal(match_keyword("enum", strlen("enum"), 0), YANG_ENUM);
    assert_int_equal(match_keyword("error-app-tag", strlen("error-app-tag"), 0), YANG_ERROR_APP_TAG);
    assert_int_equal(match_keyword("error-message", strlen("error-message"), 0), YANG_ERROR_MESSAGE);
    assert_int_equal(match_keyword("extension", strlen("extension"), 0), YANG_EXTENSION);
    assert_int_equal(match_keyword("feature", strlen("feature"), 0), YANG_FEATURE);
    assert_int_equal(match_keyword("fraction-digits", strlen("fraction-digits"),  0), YANG_FRACTION_DIGITS);
    assert_int_equal(match_keyword("grouping", strlen("grouping"), 0), YANG_GROUPING);
    assert_int_equal(match_keyword("identity", strlen("identity"), 0), YANG_IDENTITY);
    assert_int_equal(match_keyword("if-feature", strlen("if-feature"), 0), YANG_IF_FEATURE);
    assert_int_equal(match_keyword("import", strlen("import"), 0), YANG_IMPORT);
    assert_int_equal(match_keyword("include", strlen("include"), 0), YANG_INCLUDE);
    assert_int_equal(match_keyword("input", strlen("input"), 0), YANG_INPUT);
    assert_int_equal(match_keyword("key", strlen("key"), 0), YANG_KEY);
    assert_int_equal(match_keyword("leaf", strlen("leaf"), 0), YANG_LEAF);
    assert_int_equal(match_keyword("leaf-list", strlen("leaf-list"), 0), YANG_LEAF_LIST);
    assert_int_equal(match_keyword("length", strlen("length"), 0), YANG_LENGTH);
    assert_int_equal(match_keyword("list", strlen("list"), 0), YANG_LIST);
    assert_int_equal(match_keyword("mandatory", strlen("mandatory"), 0), YANG_MANDATORY);
    assert_int_equal(match_keyword("max-elements", strlen("max-elements"), 0), YANG_MAX_ELEMENTS);
    assert_int_equal(match_keyword("min-elements", strlen("min-elements"), 0), YANG_MIN_ELEMENTS);
    assert_int_equal(match_keyword("modifier", strlen("modifier"), 0), YANG_MODIFIER);
    assert_int_equal(match_keyword("module", strlen("module"), 0), YANG_MODULE);
    assert_int_equal(match_keyword("must", strlen("must"), 0), YANG_MUST);
    assert_int_equal(match_keyword("namespace", strlen("namespace"), 0), YANG_NAMESPACE);
    assert_int_equal(match_keyword("notification", strlen("notification"), 0), YANG_NOTIFICATION);
    assert_int_equal(match_keyword("ordered-by", strlen("ordered-by"), 0), YANG_ORDERED_BY);
    assert_int_equal(match_keyword("organization", strlen("organization"), 0), YANG_ORGANIZATION);
    assert_int_equal(match_keyword("output", strlen("output"), 0), YANG_OUTPUT);
    assert_int_equal(match_keyword("path", strlen("path"), 0), YANG_PATH);
    assert_int_equal(match_keyword("pattern", strlen("pattern"), 0), YANG_PATTERN);
    assert_int_equal(match_keyword("position", strlen("position"), 0), YANG_POSITION);
    assert_int_equal(match_keyword("prefix", strlen("prefix"), 0), YANG_PREFIX);
    assert_int_equal(match_keyword("presence", strlen("presence"), 0), YANG_PRESENCE);
    assert_int_equal(match_keyword("range", strlen("range"), 0), YANG_RANGE);
    assert_int_equal(match_keyword("reference", strlen("reference"), 0), YANG_REFERENCE);
    assert_int_equal(match_keyword("refine", strlen("refine"), 0), YANG_REFINE);
    assert_int_equal(match_keyword("require-instance", strlen("require-instance"), 0), YANG_REQUIRE_INSTANCE);
    assert_int_equal(match_keyword("revision", strlen("revision"), 0), YANG_REVISION);
    assert_int_equal(match_keyword("revision-date", strlen("revision-date"), 0), YANG_REVISION_DATE);
    assert_int_equal(match_keyword("rpc", strlen("rpc"), 0), YANG_RPC);
    assert_int_equal(match_keyword("status", strlen("status"), 0), YANG_STATUS);
    assert_int_equal(match_keyword("submodule", strlen("submodule"), 0), YANG_SUBMODULE);
    assert_int_equal(match_keyword("type", strlen("type"), 0), YANG_TYPE);
    assert_int_equal(match_keyword("typedef", strlen("typedef"), 0), YANG_TYPEDEF);
    assert_int_equal(match_keyword("unique", strlen("unique"), 0), YANG_UNIQUE);
    assert_int_equal(match_keyword("units", strlen("units"), 0), YANG_UNITS);
    assert_int_equal(match_keyword("uses", strlen("uses"), 0), YANG_USES);
    assert_int_equal(match_keyword("value", strlen("value"), 0), YANG_VALUE);
    assert_int_equal(match_keyword("when", strlen("when"), 0), YANG_WHEN);
    assert_int_equal(match_keyword("yang-version", strlen("yang-version"), 0), YANG_YANG_VERSION);
    assert_int_equal(match_keyword("yin-element", strlen("yin-element"), 0), YANG_YIN_ELEMENT);
}

static void
test_match_argument(void **state)
{
    (void)state; /* unused */

    assert_int_equal(match_argument_name("", 5), YIN_ARG_NONE);
    assert_int_equal(match_argument_name("qwertyasd", 5), YIN_ARG_NONE);
    assert_int_equal(match_argument_name("conditionasd", 8), YIN_ARG_NONE);
    assert_int_equal(match_argument_name("condition", 9), YIN_ARG_CONDITION);
    assert_int_equal(match_argument_name("date", 4), YIN_ARG_DATE);
    assert_int_equal(match_argument_name("module", 6), YIN_ARG_MODULE);
    assert_int_equal(match_argument_name("name", 4), YIN_ARG_NAME);
    assert_int_equal(match_argument_name("tag", 3), YIN_ARG_TAG);
    assert_int_equal(match_argument_name("target-node", 11), YIN_ARG_TARGET_NODE);
    assert_int_equal(match_argument_name("text", 4), YIN_ARG_TEXT);
    assert_int_equal(match_argument_name("uri", 3), YIN_ARG_URI);
    assert_int_equal(match_argument_name("value", 5), YIN_ARG_VALUE);
}

static void
test_meta(void **state)
{
    LY_ERR ret = LY_SUCCESS;
    struct state *st = *state;

    ret = yin_parse_module(st->ctx,"<module name=\"example-foo\">\
                                        <organization xmlns=\"urn:ietf:params:xml:ns:yang:yin:1\">organiz&lt;ation...</organization>\
                                        <contact>contact...</contact>\
                                        <description>description...</description>\
                                        <reference>reference...</reference>\
                                    </module>", st->mod);

    assert_int_equal(ret, LY_SUCCESS);
    assert_string_equal(st->mod->parsed->mod->org, "organiz<ation...");
    assert_string_equal(st->mod->parsed->mod->contact, "contact...");
    assert_string_equal(st->mod->parsed->mod->dsc, "description...");
    assert_string_equal(st->mod->parsed->mod->ref, "reference...");
}

int
main(void)
{

    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_parse, setup_f, teardown_f),
        cmocka_unit_test_setup_teardown(test_meta, setup_f, teardown_f),
        cmocka_unit_test(test_match_keyword),
        cmocka_unit_test(test_match_argument),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
