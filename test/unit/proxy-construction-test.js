const zmq = require("../..")
const semver = require("semver")
const {assert} = require("chai")
const {EventEmitter} = require("events")

describe("proxy construction", function() {
  beforeEach(function() {
    /* ZMQ < 4.0.5 has no steerable proxy support. */
    if (semver.satisfies(zmq.version, "< 4.0.5")) this.skip()
  })

  afterEach(function() {
    gc()
  })

  describe("with constructor", function() {
    it("should throw if called as function", function() {
      assert.throws(
        () => zmq.Proxy(),
        TypeError,
        "Class constructors cannot be invoked without 'new'",
      )
    })

    it("should throw with too few arguments", function() {
      assert.throws(
        () => new zmq.Proxy,
        TypeError,
        "Front-end must be a socket object",
      )
    })

    it("should throw with too many arguments", function() {
      assert.throws(
        () => new zmq.Proxy(new zmq.Dealer, new zmq.Dealer, new zmq.Dealer),
        TypeError,
        "Expected 2 arguments",
      )
    })

    it("should throw with invalid socket", function() {
      try {
        new zmq.Proxy({}, {})
        assert.ok(false)
      } catch (err) {
        assert.instanceOf(err, Error)
        assert.oneOf(err.message, [
          "Invalid pointer passed as argument", /* before 8.7 */
          "Invalid argument", /* as of 8.7 */
        ])
      }
    })
  })
})
