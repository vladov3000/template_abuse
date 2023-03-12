
// A term is either a Hook or Arrow
// Each template paremeter should be a term.
template <typename> struct Hook {};
template <typename, typename> struct Arrow {};

// These 'tag' each step of our proof.
struct Axiom1 {}; struct Axiom2 {}; struct Axiom3 {};
template <typename, typename> struct ModusPonens {};

// Proof interprets the tags defined above, and returns whether
// they form a valid proof in Proof::valid.
// The first template parameter is the term we want to prove is true.
// The second template is one of the tags defined above.
template <typename, typename>
struct Proof { enum { valid = false }; static_assert(valid); };

// Our first template specialization of Proof describes that if
// the term has the form 'A -> B -> A', it is valid.
template <typename A, typename B>
struct Proof<Arrow<A, Arrow<B, A>>, Axiom1> { enum { valid = true }; };

// Axiom 2 is (A -> B -> C) -> (A -> B) -> A -> C
template <typename A, typename B, typename C>
struct Proof<Arrow<Arrow<A, Arrow<B, C>>, Arrow<Arrow<A, B>, Arrow<A, C>>>, Axiom2> {
  enum { valid = true };
};

// Axiom 3 is (~A -> ~B) -> B -> A
template <typename A, typename B>
struct Proof<Arrow<Arrow<Hook<A>, Hook<B>>, Arrow<B, A>>, Axiom3> {
  enum { valid = true };
};

// Finally, ModusPonens states that if we have valid proofs of A and
// A -> B, we know that B is also valid.
template <typename A, typename B, typename P, typename Q>
struct Proof<B, ModusPonens<Proof<A, P>, Proof<Arrow<A, B>, Q>>> {
  enum { valid = Proof<A, P>::valid && Proof<Arrow<A, B>, Q>::valid };
  static_assert(valid);
};

// This is a simple proof of self implication i.e. that A -> A
template <typename A>
using SelfImplication =
  Proof<Arrow<A, A>, ModusPonens
        <Proof<Arrow<A, Arrow<A, A>>, Axiom1>,
         Proof<Arrow<Arrow<A, Arrow<A, A>>, Arrow<A, A>>, ModusPonens
               <Proof<Arrow<A, Arrow<Arrow<A, A>, A>>, Axiom1>,
                Proof<Arrow<Arrow<A, Arrow<Arrow<A, A>, A>>, Arrow<Arrow<A, Arrow<A, A>>, Arrow<A, A>>>, Axiom2>>>>>;

// We have instantiate the proof to check it.
static_assert(SelfImplication<int>::valid);

int main() {}
