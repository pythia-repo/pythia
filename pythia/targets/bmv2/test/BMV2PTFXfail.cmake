# XFAILS: tests that *temporarily* fail
# =====================================
# Xfails are _temporary_ failures: the tests should work but we haven't fixed p4pythia yet.

# TODO: For these test we should add the --permissive flag.
pythia_add_xfail_reason(
  "pythia-p4c-bmv2-ptf"
  "The validity bit of .* is tainted"
)

pythia_add_xfail_reason(
  "pythia-p4c-bmv2-ptf"
  "Assert/assume can not be executed under a tainted condition"
  # Assert/Assume error: assert/assume(false).
  bmv2_assert.p4
)
