/******************************************************************************
 *
 * This file is part of the CAROM distribution.  For full copyright
 * information, see COPYRIGHT.
 *
 * Copyright:   (c) 2013-2014 Lawrence Livermore National Security, LLC
 * Description: The concrete implementation of the incremental SVD algorithm
 *              using Matthew Brand's "fast update" method.
 *
 *****************************************************************************/

#ifndef included_IncrementalSVDFastUpdate_h
#define included_IncrementalSVDFastUpdate_h

#include "IncrementalSVD.h"

namespace CAROM {

/**
 * IncrementalSVDFastUpdate implements Brand's fast update incremental SVD
 * algorithm by implementing the pure virtual methods of the IncrementalSVD
 * base class.
 */
class IncrementalSVDFastUpdate : public IncrementalSVD
{
   public:
      /**
       * @brief Constructor.
       *
       * @pre dim > 0
       * @pre redundancy_tol > 0.0
       * @pre sampling_tol > 0.0
       * @pre samples_per_time_interval > 0
       *
       * @param[in] dim The dimension of the system on this processor.
       * @param[in] redundancy_tol Tolerance to determine if a sample is
       *                           redundant or not.
       * @param[in] skip_redundant If true skip redundant samples.
       * @param[in] sampling_tol Sampling control tolerance.  Limits error in
       *                         projection of sample into reduced order space
       *                         followed by a lift back to full order space.
       * @param[in] samples_per_time_interval The number of samples to be
       *                                      collected for each time interval.
       * @param[in] debug_algorithm If true results of the algorithm will be
       *                            printed to facilitate debugging.
       */
      IncrementalSVDFastUpdate(
         int dim,
         double redundancy_tol,
         bool skip_redundant,
         double sampling_tol,
         int samples_per_time_interval,
         bool debug_algorithm = false);

      /**
       * @brief Destructor.
       */
      ~IncrementalSVDFastUpdate();

   private:
      /**
       * @brief Unimplemented default constructor.
       */
      IncrementalSVDFastUpdate();

      /**
       * @brief Unimplemented copy constructor.
       */
      IncrementalSVDFastUpdate(
         const IncrementalSVDFastUpdate& other);

      /**
       * @brief Unimplemented assignment operator.
       */
      IncrementalSVDFastUpdate&
      operator = (
         const IncrementalSVDFastUpdate& rhs);

      /**
       * @brief Constructs the first svd.
       *
       * @pre u != 0
       * @pre time >= 0.0
       *
       * @param[in] u The first state.
       * @param[in] time The simulation time for the first state.
       */
      virtual
      void
      buildInitialSVD(
         const double* u,
         double time);

      /**
       * @brief Computes the current basis vectors.
       */
      virtual
      void
      computeBasis();

      /**
       * @brief Add a redundant sample to the svd.
       *
       * @pre A != 0
       * @pre sigma != 0
       *
       * @param[in] A The left singular vectors.
       * @param[in] sigma The singular values.
       */
      void
      addRedundantSample(
         const Matrix* A,
         const Matrix* sigma);

      /**
       * @brief Add a new, unique sample to the svd.
       *
       * @pre j != 0
       * @pre A != 0
       * @pre sigma != 0
       *
       * @param[in] j The new column of d_U.
       * @param[in] A The left singular vectors.
       * @param[in] sigma The singular values.
       */
      void
      addNewSample(
         const Vector* j,
         const Matrix* A,
         Matrix* sigma);

      /**
       * @brief The matrix U distributed across all processors.
       *
       * Each processor's d_U is the part of the distributed matrix local to
       * that processor.
       */
      Matrix* d_U;

      /**
       * @brief The matrix U'.
       *
       * U' is not distributed and the entire matrix exists on each processor.
       */
      Matrix* d_Up;
};

}

#endif
