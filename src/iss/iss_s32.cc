#include "iss.h"

#include "../logger.h"

std::map<std::string, PtxRegister> &Iss::S32Iss(const PtxInstruction &i, std::map<std::string, PtxRegister> &regs) {
  int32_t src0;
  int32_t src1;
  int32_t src2;
  int32_t dst;
  uint32_t dst_u32;
  int64_t double_size_dst;
  bool pred;

  switch (i.GetInstructionKeyword()) {
    // Integer Arithmetic Instructions
    case kAdd:
      //TODO: sat for preventing overflow
      src0 = CastWrapperS32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperS32(regs[i.GetSrcRegister()[1]]);

      dst = src0 + src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kS32);
      break;
    case kSub:
      //TODO: sat for preventing overflow
      src0 = CastWrapperS32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperS32(regs[i.GetSrcRegister()[1]]);

      dst = src0 - src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kS32);
      break;
    case kMul: src0 = CastWrapperS32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperS32(regs[i.GetSrcRegister()[1]]);

      double_size_dst = static_cast<int64_t>(src0) * static_cast<int64_t>(src1);
      if (i.GetModifier()[0] == "hi") {
        regs[i.GetDstRegister()].SetValue(static_cast<int32_t>(double_size_dst >> 32));
        regs[i.GetDstRegister()].SetType(kS32);
      } else if (i.GetModifier()[0] == "lo") {
        regs[i.GetDstRegister()].SetValue(static_cast<int32_t>(double_size_dst & 2147483647));
        regs[i.GetDstRegister()].SetType(kS32);
      } else if (i.GetModifier()[0] == "wide") {
        regs[i.GetDstRegister()].SetValue(double_size_dst);
        regs[i.GetDstRegister()].SetType(kS64);
      } else {
        Logger::Log("Iss::S32Iss | Instruction has no valid modifier \"" + i.GetRawLine() + "\"\n", "ERROR");
      }
      break;
    case kMad: src0 = CastWrapperS32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperS32(regs[i.GetSrcRegister()[1]]);
      src2 = CastWrapperS32(regs[i.GetSrcRegister()[2]]);

      double_size_dst = static_cast<int64_t>(src0) * static_cast<int64_t>(src1);
      if (i.GetModifier()[0] == "hi") {
        // TODO: sat for overflow prevention
        regs[i.GetDstRegister()].SetValue(static_cast<int32_t>(double_size_dst >> 32) + src2);
        regs[i.GetDstRegister()].SetType(kS32);
      } else if (i.GetModifier()[0] == "lo") {
        regs[i.GetDstRegister()].SetValue(static_cast<int32_t>(double_size_dst & 2147483647) + src2);
        regs[i.GetDstRegister()].SetType(kS32);
      } else if (i.GetModifier()[0] == "wide") {
        regs[i.GetDstRegister()].SetValue(double_size_dst + src2);
        regs[i.GetDstRegister()].SetType(kS64);
      } else {
        Logger::Log("Iss::S32Iss | Instruction has no valid modifier \"" + i.GetRawLine() + "\"\n", "ERROR");
      }
      break;
    case kMul24: src0 = CastWrapperS32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperS32(regs[i.GetSrcRegister()[1]]);

      double_size_dst = static_cast<int64_t>(src0) * static_cast<int64_t>(src1);
      if (i.GetModifier()[0] == "hi") {
        regs[i.GetDstRegister()].SetValue(static_cast<int32_t>((double_size_dst >> 16) & 2147483647));
        regs[i.GetDstRegister()].SetType(kS32);
      } else if (i.GetModifier()[0] == "lo") {
        regs[i.GetDstRegister()].SetValue(static_cast<int32_t>(double_size_dst & 2147483647));
        regs[i.GetDstRegister()].SetType(kS32);
      } else {
        Logger::Log("Iss::S32Iss | Instruction has no valid modifier \"" + i.GetRawLine() + "\"\n", "ERROR");
      }
      break;
    case kMad24: src0 = CastWrapperS32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperS32(regs[i.GetSrcRegister()[1]]);
      src2 = CastWrapperS32(regs[i.GetSrcRegister()[2]]);

      double_size_dst = static_cast<int64_t>(src0) * static_cast<int64_t>(src1);
      if (i.GetModifier()[0] == "hi") {
        // TODO: sat for overflow prevention
        regs[i.GetDstRegister()].SetValue(static_cast<int32_t>((double_size_dst >> 16) & 2147483647) + src2);
        regs[i.GetDstRegister()].SetType(kS32);
      } else if (i.GetModifier()[0] == "lo") {
        regs[i.GetDstRegister()].SetValue(static_cast<int32_t>(double_size_dst & 2147483647) + src2);
        regs[i.GetDstRegister()].SetType(kS32);
      } else {
        Logger::Log("Iss::S32Iss | Instruction has no valid modifier \"" + i.GetRawLine() + "\"\n", "ERROR");
      }
      break;
    case kSad: src0 = CastWrapperS32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperS32(regs[i.GetSrcRegister()[1]]);
      src2 = CastWrapperS32(regs[i.GetSrcRegister()[2]]);

      dst = (src0 < src1 ? src1 - src0 : src0 - src1) + src2;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kS32);
      break;
    case kDiv: src0 = CastWrapperS32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperS32(regs[i.GetSrcRegister()[1]]);

      dst = src0 / src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kS32);
      break;
    case kRem: src0 = CastWrapperS32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperS32(regs[i.GetSrcRegister()[1]]);

      dst = src0 % src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kS32);
      break;
    case kAbs: src0 = CastWrapperS32(regs[i.GetSrcRegister()[0]]);

      dst = std::abs(src0);
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kS32);
      break;
    case kNeg: src0 = CastWrapperS32(regs[i.GetSrcRegister()[0]]);

      dst = -src0;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kS32);
      break;
    case kMin: src0 = CastWrapperS32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperS32(regs[i.GetSrcRegister()[1]]);

      dst = src0 < src1 ? src0 : src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kS32);
      break;
    case kMax: src0 = CastWrapperS32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperS32(regs[i.GetSrcRegister()[1]]);

      dst = src0 > src1 ? src0 : src1;
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kS32);
      break;
    case kBfind:
      //TODO: shiftamt
      src0 = CastWrapperS32(regs[i.GetSrcRegister()[0]]);

      dst_u32 = 4294967295;
      for (int j = 31; j > -1; j--) {
        if ((src0 >> j) & 1) {
          dst_u32 = j;
          break;
        }
      }
      regs[i.GetDstRegister()].SetValue(dst_u32);
      regs[i.GetDstRegister()].SetType(kS32);
      break;
      // Comparison and Selection Instructions
      // ..
    case kSetp: src0 = CastWrapperS32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperS32(regs[i.GetSrcRegister()[1]]);

      // TODO: Missing
      if (i.GetModifier()[0] == "eq") {
        pred = src0 == src1;
      } else if (i.GetModifier()[0] == "ne") {
        pred = src0 != src1;
      } else if (i.GetModifier()[0] == "lt") {
        pred = src0 < src1;
      } else if (i.GetModifier()[0] == "le") {
        pred = src0 <= src1;
      } else if (i.GetModifier()[0] == "gt") {
        pred = src0 > src1;
      } else if (i.GetModifier()[0] == "ge") {
        pred = src0 >= src1;
      }

      regs[i.GetDstRegister()].SetValue(pred);
      regs[i.GetDstRegister()].SetType(kPred);
      break;
      // Data Movement and Conversion Instructions
      // ...
    case kCvt: src0 = CastWrapperS32(regs[i.GetSrcRegister()[0]]);

      if (i.GetModifier()[i.GetModifier().size() - 2] == "u8") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint8_t>(src0));
        regs[i.GetDstRegister()].SetType(kU8);
      } else if (i.GetModifier()[i.GetModifier().size() - 2] == "u16") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint16_t>(src0));
        regs[i.GetDstRegister()].SetType(kU16);
      } else if (i.GetModifier()[i.GetModifier().size() - 2] == "u32") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint32_t>(src0));
        regs[i.GetDstRegister()].SetType(kU32);
      } else if (i.GetModifier()[i.GetModifier().size() - 2] == "u64") {
        regs[i.GetDstRegister()].SetValue(static_cast<uint64_t>(src0));
        regs[i.GetDstRegister()].SetType(kU64);
      } else if (i.GetModifier()[i.GetModifier().size() - 2] == "s8") {
        regs[i.GetDstRegister()].SetValue(static_cast<int8_t>(src0));
        regs[i.GetDstRegister()].SetType(kS8);
      } else if (i.GetModifier()[i.GetModifier().size() - 2] == "s16") {
        regs[i.GetDstRegister()].SetValue(static_cast<int16_t>(src0));
        regs[i.GetDstRegister()].SetType(kS16);
      } else if (i.GetModifier()[i.GetModifier().size() - 2] == "s32") {
        regs[i.GetDstRegister()].SetValue(static_cast<int32_t>(src0));
        regs[i.GetDstRegister()].SetType(kS32);
      } else if (i.GetModifier()[i.GetModifier().size() - 2] == "s64") {
        regs[i.GetDstRegister()].SetValue(static_cast<int64_t>(src0));
        regs[i.GetDstRegister()].SetType(kS64);
      } else {
        Logger::Log("Iss::S32Iss | Conversion from s32 to " + i.GetModifier()[i.GetModifier().size() - 2]
                        + " not supported yet\n", "ERROR");
      }
      /*} else if (i.GetModifier()[i.GetModifier().size()-2] == "f16") {
        regs[i.GetDstRegister()].SetValue(static_cast<float>(src0));
      } else if (i.GetModifier()[i.GetModifier().size()-2] == "f32") {
        regs[i.GetDstRegister()].SetValue(static_cast<float>(src0));
      } else if (i.GetModifier()[i.GetModifier().size()-2] == "f64") {
        regs[i.GetDstRegister()].SetValue(static_cast<float>(src0));
      }*/
      break;
      // Comparison and Selection Instructions
    case kSelp:src0 = CastWrapperS32(regs[i.GetSrcRegister()[0]]);
      src1 = CastWrapperS32(regs[i.GetSrcRegister()[1]]);
      src2 = CastWrapperS32(regs[i.GetSrcRegister()[2]]);

      if (src2) {
        dst = src0;
      } else {
        dst = src1;
      }
      regs[i.GetDstRegister()].SetValue(dst);
      regs[i.GetDstRegister()].SetType(kS32);
      break;
    default: Logger::Log("Iss::S32Iss | Instruction not implemented: \"" + i.GetRawLine() + "\"\n", "ERROR");
  }

  return regs;
}

